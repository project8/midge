#include "rt_rtf_power_transformer.hh"
//#include "coremsg.hh"

#include "fourier.hh"

#include <cmath>

namespace midge
{

    rt_rtf_power_transformer::rt_rtf_power_transformer() :
            f_impedance_ohm( 1. ),
            f_begin_hz( 0. ),
            f_end_hz( 1.e9 ),
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

        rt_data* t_in_data;
        rtf_data* t_out_data;

        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;
        real_t t_frequency_interval;

        count_t t_begin;
        count_t t_end;
        count_t t_under;
        count_t t_center;

        real_t t_norm;

        fourier* t_fourier = fourier::get_instance();
        complex_t* t_signal = NULL;
        complex_t* t_transform = NULL;
        fourier_t* t_generator = NULL;

        real_t t_power_watt;

        while( true )
        {
            t_in_command = in_stream< 0 >().get();
            t_in_data = in_stream< 0 >().data();
            t_out_data = out_stream< 0 >().data();

            if( t_in_command == stream::s_start )
            {
                t_size = t_in_data->get_size();
                t_time_interval = t_in_data->get_time_interval();
                t_time_index = t_in_data->get_time_index();
                t_frequency_interval = 1. / (t_size * t_time_interval);

                if( t_size % 2 == 0 )
                {
                    t_under = (t_size / 2) - 1;
                    t_center = t_size / 2;
                }
                else
                {
                    t_under = t_size / 2;
                    t_center = 0;
                }

                t_begin = (count_t) (round( f_begin_hz / t_frequency_interval ));
                t_end = (count_t) (round( f_end_hz / t_frequency_interval ));
                if( t_end > t_under )
                {
                    t_end = t_under;
                }
                if( t_begin > t_under )
                {
                    t_begin = t_under;
                }
                if( t_begin < 1 )
                {
                    t_begin = 1;
                }

                f_window->set_size( t_size );
                t_norm = 1. / (f_impedance_ohm * f_window->sum() * f_window->sum());

                t_signal = t_fourier->allocate< complex_t >( t_size );
                t_transform = t_fourier->allocate< complex_t >( t_size );
                t_generator = t_fourier->forward( t_size, t_signal, t_transform );

                t_out_data->set_size( t_end - t_begin + 1 );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index + t_under + 1 );
                t_out_data->set_frequency_interval( t_frequency_interval );
                t_out_data->set_frequency_index( t_begin );

                //coremsg( s_normal ) << "rt rtf power transformer <" << get_name() << "> pushing <start>" << ret;
                //coremsg( s_normal ) << "  size <" << t_out_data->get_size() << ">" << ret;
                //coremsg( s_normal ) << "  time interval <" << t_out_data->get_time_interval() << ">" << ret;
                //coremsg( s_normal ) << "  time index <" << t_out_data->get_time_index() << ">" << ret;
                //coremsg( s_normal ) << "  frequency interval <" << t_out_data->get_frequency_interval() << ">" << ret;
                //coremsg( s_normal ) << "  frequency index <" << t_out_data->get_frequency_index() << ">" << eom;
                out_stream< 0 >().set( stream::s_start );
                continue;
            }
            if( t_in_command == stream::s_run )
            {
                t_time_index = t_in_data->get_time_index();

                t_out_data->set_size( t_end - t_begin + 1 );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index + t_under + 1 );
                t_out_data->set_frequency_interval( t_frequency_interval );
                t_out_data->set_frequency_index( t_begin );

                for( t_index = 0; t_index < t_size; t_index++ )
                {
                    t_signal[ t_index ][ 0 ] = t_in_data->at( t_index ) * f_window->at( t_index );
                    t_signal[ t_index ][ 1 ] = 0.;
                }

                t_fourier->execute( t_generator );

                for( t_index = t_begin; t_index <= t_end; t_index++ )
                {
                    t_power_watt = 2. * (t_transform[ t_index ][ 0 ] * t_transform[ t_index ][ 0 ] + t_transform[ t_index ][ 1 ] * t_transform[ t_index ][ 1 ]) * t_norm;
                    t_out_data->at( t_index - t_begin ) = t_power_watt;
                }

                //coremsg( s_normal ) << "rt rtf power transformer <" << get_name() << "> pushing <run>" << ret;
                //coremsg( s_normal ) << "  size <" << t_out_data->get_size() << ">" << ret;
                //coremsg( s_normal ) << "  time interval <" << t_out_data->get_time_interval() << ">" << ret;
                //coremsg( s_normal ) << "  time index <" << t_out_data->get_time_index() << ">" << ret;
                //coremsg( s_normal ) << "  frequency interval <" << t_out_data->get_frequency_interval() << ">" << ret;
                //coremsg( s_normal ) << "  frequency index <" << t_out_data->get_frequency_index() << ">" << eom;
                out_stream< 0 >().set( stream::s_run );
                continue;
            }
            if( t_in_command == stream::s_stop )
            {
                t_fourier->free< complex_t >( t_signal );
                t_fourier->free< complex_t >( t_transform );
                t_fourier->destroy( t_generator );

                //coremsg( s_normal ) << "rt rtf power transformer <" << get_name() << "> pushing <stop>" << ret;
                //coremsg( s_normal ) << "  size <" << t_out_data->get_size() << ">" << ret;
                //coremsg( s_normal ) << "  time interval <" << t_out_data->get_time_interval() << ">" << ret;
                //coremsg( s_normal ) << "  time index <" << t_out_data->get_time_index() << ">" << ret;
                //coremsg( s_normal ) << "  frequency interval <" << t_out_data->get_frequency_interval() << ">" << ret;
                //coremsg( s_normal ) << "  frequency index <" << t_out_data->get_frequency_index() << ">" << eom;
                out_stream< 0 >().set( stream::s_stop );
                continue;
            }
            if( t_in_command == stream::s_exit )
            {
                //coremsg( s_normal ) << "rt rtf power transformer <" << get_name() << "> pushing <exit>" << ret;
                //coremsg( s_normal ) << "  size <" << t_out_data->get_size() << ">" << ret;
                //coremsg( s_normal ) << "  time interval <" << t_out_data->get_time_interval() << ">" << ret;
                //coremsg( s_normal ) << "  time index <" << t_out_data->get_time_index() << ">" << ret;
                //coremsg( s_normal ) << "  frequency interval <" << t_out_data->get_frequency_interval() << ">" << ret;
                //coremsg( s_normal ) << "  frequency index <" << t_out_data->get_frequency_index() << ">" << eom;
                out_stream< 0 >().set( stream::s_exit );
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
