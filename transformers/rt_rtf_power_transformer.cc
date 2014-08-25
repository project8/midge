#include "rt_rtf_power_transformer.hh"

#include "fourier.hh"

#include <cmath>

namespace midge
{

    rt_rtf_power_transformer::rt_rtf_power_transformer() :
            f_impedance_ohm( 1. ),
            f_begin_hz( 0. ),
            f_end_hz( 1. ),
            f_window( NULL ),
            f_length( 10 )
    {
    }
    rt_rtf_power_transformer::~rt_rtf_power_transformer()
    {
    }

    void rt_rtf_power_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );

        if( f_window == NULL )
        {
            throw error() << "rt rf power transformer window unset";
        }

        return;
    }

    void rt_rtf_power_transformer::execute()
    {
        count_t t_index;

        command_t t_in_command;

        rt_data t_in_data;
        real_t* t_in_raw;
        count_t t_in_size;

        rtf_data t_out_data;
        real_t* t_out_raw;
        count_t t_out_size;

        real_t t_time_interval;
        count_t t_time_index;
        real_t t_frequency_interval;
        count_t t_frequency_index;

        count_t t_begin;
        count_t t_end;
        count_t t_under;
        count_t t_center;

        const real_t* t_window;
        real_t t_norm;

        fourier* t_fourier = fourier::get_instance();
        complex_t* t_signal = NULL;
        complex_t* t_transform = NULL;
        fourier_t* t_generator = NULL;

        real_t t_power_watt;
        //real_t t_power_dbm;

        while( true )
        {
            in_stream< 0 >() >> t_in_data;
            out_stream< 0 >() >> t_out_data;
            t_in_command = in_stream< 0 >().command();

            if( t_in_command == stream::s_start )
            {
                t_in_size = t_in_data.get_size();
                t_time_interval = t_in_data.get_time_interval();
                t_time_index = t_in_data.get_time_index();
                t_frequency_interval = 1. / (t_in_size * t_time_interval);
                t_frequency_index = 0;

                t_begin = (count_t) (round( f_begin_hz / t_frequency_interval ));
                t_end = (count_t) (round( f_end_hz / t_frequency_interval ));
                if( t_in_size % 2 == 0 )
                {
                    t_out_size = (t_in_size / 2) + 1;
                    t_under = (t_in_size / 2) - 1;
                    t_center = t_in_size / 2;
                }
                else
                {
                    t_out_size = (t_in_size + 1) / 2;
                    t_under = t_in_size / 2;
                    t_center = 0;
                }

                f_window->initialize( t_in_size );
                t_window = f_window->raw();
                t_norm = 1. / (f_impedance_ohm * f_window->sum() * f_window->sum());

                t_signal = t_fourier->allocate< complex_t >( t_in_size );
                t_transform = t_fourier->allocate< complex_t >( t_in_size );
                t_generator = t_fourier->forward( t_in_size, t_signal, t_transform );

                t_out_data.set_size( t_out_size );
                t_out_data.set_time_interval( t_time_interval );
                t_out_data.set_time_index( t_time_index );
                t_out_data.set_frequency_interval( t_frequency_interval );
                t_out_data.set_frequency_index( t_frequency_index );

                out_stream< 0 >().command( stream::s_start );
                out_stream< 0 >() << t_out_data;
                in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_run )
            {
                t_time_index = t_in_data.get_time_index();
                t_in_raw = t_in_data.raw();

                t_out_data.set_size( t_out_size );
                t_out_data.set_time_interval( t_time_interval );
                t_out_data.set_time_index( t_time_index + t_under );
                t_out_data.set_frequency_interval( t_frequency_interval );
                t_out_data.set_frequency_index( t_frequency_index );
                t_out_raw = t_out_data.raw();

                for( t_index = 0; t_index < t_in_size; t_index++ )
                {
                    t_signal[ t_index ][ 0 ] = t_in_raw[ t_index ] * t_window[ t_index ];
                    t_signal[ t_index ][ 1 ] = 0.;
                }

                t_fourier->execute( t_generator );

                t_power_watt = (t_transform[ 0 ][ 0 ] * t_transform[ 0 ][ 0 ] + t_transform[ 0 ][ 1 ] * t_transform[ 0 ][ 1 ]) * t_norm;
                //t_power_dbm = 10. * log10( t_power_watt ) + 30.;
                t_out_raw[ 0 ] = t_power_watt;
                for( t_index = 1; t_index <= t_under; t_index++ )
                {
                    t_power_watt = 2. * (t_transform[ t_index ][ 0 ] * t_transform[ t_index ][ 0 ] + t_transform[ t_index ][ 1 ] * t_transform[ t_index ][ 1 ]) * t_norm;
                    //t_power_dbm = 10. * log10( t_power_watt ) + 30.;
                    t_out_raw[ t_index ] = t_power_watt;
                }
                if( t_center != 0 )
                {
                    t_power_watt = (t_transform[ t_center ][ 0 ] * t_transform[ t_center ][ 0 ] + t_transform[ t_center ][ 1 ] * t_transform[ t_center ][ 1 ]) * t_norm;
                    //t_power_dbm = 10. * log10( t_power_watt ) + 30.;
                    t_out_raw[ t_center ] = t_power_watt;
                }

                out_stream< 0 >().command( stream::s_run );
                out_stream< 0 >() << t_out_data;
                in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_stop )
            {
                t_fourier->free< complex_t >( t_signal );
                t_fourier->free< complex_t >( t_transform );
                t_fourier->destroy( t_generator );

                out_stream< 0 >().command( stream::s_stop );
                out_stream< 0 >() << t_out_data;
                in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_exit )
            {
                out_stream< 0 >().command( stream::s_exit );
                out_stream< 0 >() << t_out_data;
                in_stream< 0 >() << t_in_data;
                return;
            }
        }

        return;
    }

    void rt_rtf_power_transformer::finalize()
    {
        out_buffer< 0 >().finalize();

        delete f_window;

        return;
    }

}
