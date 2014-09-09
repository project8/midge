#include "rt_sinusoidal_producer.hh"

#include <cmath>

namespace midge
{

    rt_sinusoidal_producer::rt_sinusoidal_producer() :
            f_impedance_ohm( 1. ),
            f_carrier_power_dbm( 0. ),
            f_carrier_frequency_hz( 0. ),
            f_carrier_phase_deg( 0. ),
            f_signal_amplitude_hz( 1. ),
            f_signal_frequency_hz( 1. ),
            f_signal_phase_deg( 0. ),
            f_begin_sec( 0. ),
            f_end_sec( 1. ),
            f_start_sec( 0. ),
            f_stop_sec( 1. ),
            f_interval_sec( 1. ),
            f_length( 10 ),
            f_size( 0 ),
            f_stride( 0 )
    {
    }
    rt_sinusoidal_producer::~rt_sinusoidal_producer()
    {
    }

    void rt_sinusoidal_producer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );
        return;
    }

    void rt_sinusoidal_producer::execute()
    {
        index_t t_index;

        rt_data* t_current_data;
        const rt_data* t_previous_data;

        count_t t_begin = (count_t) (round( f_begin_sec / f_interval_sec ));
        count_t t_end = (count_t) (round( f_end_sec / f_interval_sec ));
        count_t t_start = (count_t) (round( f_start_sec / f_interval_sec ));
        count_t t_stop = (count_t) (round( f_stop_sec / f_interval_sec ));
        count_t t_first_unwritten_index;
        count_t t_first_requested_index;

        real_t t_carrier_amplitude = sqrt( 2. * f_impedance_ohm ) * pow( 10., (f_carrier_power_dbm - 30.) / 20. );
        real_t t_carrier_linear = f_interval_sec * f_carrier_frequency_hz;
        real_t t_carrier_phase = (M_PI / 180.) * f_carrier_phase_deg;
        real_t t_signal_amplitude = f_signal_amplitude_hz / f_signal_frequency_hz;
        real_t t_signal_linear = f_interval_sec * f_signal_frequency_hz;
        real_t t_signal_phase = (M_PI / 180.) * f_signal_phase_deg;

        t_current_data = out_stream< 0 >().data();
        t_current_data->set_size( f_size );
        t_current_data->set_time_interval( f_interval_sec );
        t_current_data->set_time_index( t_begin );
        out_stream< 0 >().set( stream::s_start );

        t_first_unwritten_index = 0;
        t_first_requested_index = t_begin;
        while( true )
        {
            if( (out_stream< 0 >().get() == stream::s_stop) || (t_first_unwritten_index >= t_end) )
            {
                out_stream< 0 >().set( stream::s_stop );
                out_stream< 0 >().set( stream::s_exit );
                return;
            }

            t_current_data = out_stream< 0 >().data();

            t_current_data->set_size( f_size );
            t_current_data->set_time_interval( f_interval_sec );
            t_current_data->set_time_index( t_first_requested_index );

            if( t_first_unwritten_index > t_first_requested_index )
            {
                for( t_index = t_first_requested_index; t_index < t_first_unwritten_index; t_index++ )
                {
                    t_current_data->at( t_index - t_first_requested_index ) = t_previous_data->at( t_index - t_first_unwritten_index + f_size );
                }
                for( t_index = t_first_unwritten_index; t_index < t_first_requested_index + f_size; t_index++ )
                {
                    if( (t_index >= t_start) && (t_index <= t_stop) )
                    {
                        t_current_data->at( t_index - t_first_requested_index ) = t_carrier_amplitude * cos( 2 * M_PI * (t_carrier_linear * (t_index - t_start)) + t_carrier_phase + t_signal_amplitude * sin( 2 * M_PI * ( t_signal_linear * (t_index - t_start)) + t_signal_phase ));
                    }
                    else
                    {
                        t_current_data->at( t_index - t_first_requested_index ) = 0.;
                    }
                }
            }
            else
            {
                for( t_index = t_first_requested_index; t_index < t_first_requested_index + f_size; t_index++ )
                {
                    if( (t_index >= t_start) && (t_index <= t_stop) )
                    {
                        t_current_data->at( t_index - t_first_requested_index ) = t_carrier_amplitude * cos( 2 * M_PI * (t_carrier_linear * (t_index - t_start)) + t_carrier_phase + t_signal_amplitude * sin( 2 * M_PI * ( t_signal_linear * (t_index - t_start)) + t_signal_phase ));
                    }
                    else
                    {
                        t_current_data->at( t_index - t_first_requested_index ) = 0.;
                    }
                }
            }

            t_first_unwritten_index = t_first_requested_index + f_size;
            t_first_requested_index = t_first_requested_index + f_stride;
            t_previous_data = t_current_data;

            out_stream< 0 >().set( stream::s_run );
        }

        return;
    }

    void rt_sinusoidal_producer::finalize()
    {
        out_buffer< 0 >().finalize();
        return;
    }

}
