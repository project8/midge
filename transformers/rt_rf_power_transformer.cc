#include "rt_rf_power_transformer.hh"

#include "fourier.hh"

#include <cmath>

#include <fstream>
using std::ofstream;

namespace midge
{

    rt_rf_power_transformer::rt_rf_power_transformer() :
            f_impedance_ohm( 1. ),
            f_window( NULL ),
            f_length( 10 )
    {
    }
    rt_rf_power_transformer::~rt_rf_power_transformer()
    {
    }

    void rt_rf_power_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );
        out_buffer< 0 >().set_name( get_name() );

        if( f_window == NULL )
        {
            throw error() << "rt rf power transformer window unset";
        }

        return;
    }

    void rt_rf_power_transformer::execute()
    {
        count_t t_index;

        state_t t_in_state;
        const rt_data* t_in_data;
        const real_t* t_in_raw;
        count_t t_in_size;

        rf_data* t_out_data;
        real_t* t_out_raw;
        count_t t_out_size;

        real_t t_time_interval;
        count_t t_time_index;
        real_t t_frequency_interval;
        count_t t_frequency_index = 0;

        count_t t_nyquist;
        count_t t_last;
        real_t t_norm;
        const real_t* t_window;

        fourier* t_fourier = fourier::get_instance();
        complex_t* t_signal = NULL;
        complex_t* t_transform = NULL;
        fourier_t* t_generator = NULL;

        real_t t_power_watt;
        real_t t_power_dbm;

        while( true )
        {
            in_stream< 0 >()++;
            t_in_state = in_stream< 0 >().state();
            t_in_data = in_stream< 0 >().data();
            t_out_data = out_stream< 0 >().data();

            if( t_in_state == stream::s_start )
            {
                t_in_size = t_in_data->get_size();
                if( t_in_size % 2 == 0 )
                {
                    t_out_size = (t_in_size / 2) + 1;
                    t_last = (t_in_size / 2) - 1;
                    t_nyquist = t_in_size / 2;
                }
                else
                {
                    t_out_size = (t_in_size + 1) / 2;
                    t_last = t_in_size / 2;
                    t_nyquist = 0;
                }

                t_time_interval = t_in_data->get_time_interval();
                t_frequency_interval = 1. / (t_in_size * t_time_interval);

                f_window->initialize( t_in_size );
                t_norm = 1. / (f_impedance_ohm * f_window->sum() * f_window->sum());
                t_window = f_window->raw();

                t_signal = t_fourier->allocate_complex( t_in_size );
                t_transform = t_fourier->allocate_complex( t_in_size );
                t_generator = t_fourier->forward( t_in_size, t_signal, t_transform );

                t_out_data->set_size( t_out_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( 0 );
                t_out_data->set_frequency_interval( t_frequency_interval );
                t_out_data->set_frequency_index( 0 );

                out_stream< 0 >().state( stream::s_start );
                t_index = out_stream< 0 >()++;

                continue;
            }
            if( t_in_state == stream::s_run )
            {
                t_time_index = t_in_data->get_time_index();
                t_in_raw = t_in_data->raw();

                t_out_data->set_size( t_out_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index + t_last );
                t_out_data->set_frequency_interval( t_frequency_interval );
                t_out_data->set_frequency_index( t_frequency_index );
                t_out_raw = t_out_data->raw();

                for( t_index = 0; t_index < t_in_size; t_index++ )
                {
                    t_signal[ t_index ][ 0 ] = t_in_raw[ t_index ] * t_window[ t_index ];
                    t_signal[ t_index ][ 1 ] = 0.;
                }

                t_fourier->execute( t_generator );

                t_power_watt = (t_transform[ 0 ][ 0 ] * t_transform[ 0 ][ 0 ] + t_transform[ 0 ][ 1 ] * t_transform[ 0 ][ 1 ]) * t_norm;
                //t_power_dbm = 10. * log10( t_power_watt ) + 30.;
                t_out_raw[ 0 ] = t_power_watt;
                for( t_index = 1; t_index <= t_last; t_index++ )
                {
                    t_power_watt = 2. * (t_transform[ t_index ][ 0 ] * t_transform[ t_index ][ 0 ] + t_transform[ t_index ][ 1 ] * t_transform[ t_index ][ 1 ]) * t_norm;
                    //t_power_dbm = 10. * log10( t_power_watt ) + 30.;
                    t_out_raw[ t_index ] = t_power_watt;
                }
                if( t_nyquist != 0 )
                {
                    t_power_watt = (t_transform[ t_nyquist ][ 0 ] * t_transform[ t_nyquist ][ 0 ] + t_transform[ t_nyquist ][ 1 ] * t_transform[ t_nyquist ][ 1 ]) * t_norm;
                    //t_power_dbm = 10. * log10( t_power_watt ) + 30.;
                    t_out_raw[ t_nyquist ] = t_power_watt;
                }

                out_stream< 0 >().state( stream::s_run );
                t_index = out_stream< 0 >()++;

                continue;
            }
            if( t_in_state == stream::s_stop )
            {
                t_fourier->free_complex( t_signal );
                t_fourier->free_complex( t_transform );
                t_fourier->destroy( t_generator );

                out_stream< 0 >().state( stream::s_stop );
                t_index = out_stream< 0 >()++;

                continue;
            }
            if( t_in_state == stream::s_exit )
            {
                out_stream< 0 >().state( stream::s_exit );
                t_index = out_stream< 0 >()++;

                break;
            }
        }

        return;
    }

    void rt_rf_power_transformer::finalize()
    {
        out_buffer< 0 >().finalize();

        delete f_window;

        return;
    }

}
