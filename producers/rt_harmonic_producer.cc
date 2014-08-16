#include "rt_harmonic_producer.hh"

#include <cmath>

namespace midge
{

    rt_harmonic_producer::rt_harmonic_producer() :
            f_power_dbm( 0. ),
            f_impedance_ohm( 1. ),
            f_frequency_hz( 1. ),
            f_phase_deg( 0. ),
            f_start_sec( 0. ),
            f_stop_sec( 1. ),
            f_interval_sec( 1. ),
            f_length( 10 ),
            f_size( 0 ),
            f_stride( 0 ),
            f_start( 0 ),
            f_stop( 0 ),
            f_current( 0 ),
            f_next( 0 ),
            f_amplitude( 0. ),
            f_linear( 0. ),
            f_phase( 0. )
    {
    }
    rt_harmonic_producer::~rt_harmonic_producer()
    {
    }

    void rt_harmonic_producer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );

        f_start = (count_t) (round( f_start_sec / f_interval_sec ));
        f_stop = (count_t) (round( f_stop_sec / f_interval_sec ));
        f_current = 0;
        f_next = 0;
        f_amplitude = sqrt( 2. * f_impedance_ohm ) * pow( 10., (f_power_dbm - 30.) / 20. );
        f_linear = f_interval_sec * f_frequency_hz;
        f_phase = (M_PI / 180.) * f_phase_deg;

        return;
    }

    void rt_harmonic_producer::execute()
    {
        rt_data* t_data;
        real_t* t_raw;
        state_t t_state;
        count_t t_index;

        out_stream< 0 >() < stream::s_start;
        out_stream< 0 >()++;

        while( true )
        {
            out_stream< 0 >() > t_state;
            if( t_state == stream::s_exit )
            {
                break;
            }
            if( t_state == stream::s_stop )
            {
                out_stream< 0 >() < stream::s_stop;
                out_stream< 0 >()++;
                break;
            }

            out_stream< 0 >() < stream::s_run;

            out_stream< 0 >() >> t_data;

            if( f_current != 0 )
            {
                f_next += f_stride;
            }

            t_data->set_size( f_size );
            t_data->set_interval( f_interval_sec );
            t_data->set_time( f_next * f_interval_sec );
            t_raw = t_data->raw();

            if( f_current > f_next )
            {
                for( t_index = f_next; t_index < f_current; t_index++ )
                {
                    t_raw[ t_index - f_next ] = t_raw[ t_index - f_current + f_size ];
                }
                for( t_index = f_current; t_index < f_next + f_size; t_index++ )
                {
                    if( (t_index >= f_start) && (t_index <= f_stop) )
                    {
                        t_raw[ t_index - f_next ] = f_amplitude * cos( 2. * M_PI * f_linear * (t_index - f_start) + f_phase );
                    }
                    else
                    {
                        t_raw[ t_index - f_next ] = 0.;
                    }
                }
            }
            else
            {
                for( t_index = f_next; t_index < f_next + f_size; t_index++ )
                {
                    if( (t_index >= f_start) && (t_index <= f_stop) )
                    {
                        t_raw[ t_index - f_next ] = f_amplitude * cos( 2. * M_PI * f_linear * (t_index - f_next - f_start) + f_phase );
                    }
                    else
                    {
                        t_raw[ t_index - f_next ] = 0.;
                    }
                }
            }

            f_current = f_next + f_size;

            out_stream< 0 >() << t_data;

            if( f_current >= f_stop )
            {
                out_stream< 0 >() < stream::s_stop;
                out_stream< 0 >()++;
                break;
            }
        }

        out_stream< 0 >() < stream::s_exit;
        out_stream< 0 >()++;

        return;
    }

    void rt_harmonic_producer::finalize()
    {
        f_start = 0.;
        f_stop = 0.;
        f_current = 0;
        f_next = 0;
        f_amplitude = 0.;
        f_linear = 0.;
        f_phase = 0.;

        return;
    }

}
