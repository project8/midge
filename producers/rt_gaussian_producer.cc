#include "rt_gaussian_producer.hh"

#include "random.hh"

#include <cmath>

namespace midge
{

    rt_gaussian_producer::rt_gaussian_producer() :
            f_power_dbm( 0. ),
            f_impedance_ohm( 1. ),
            f_begin_sec( 0. ),
            f_end_sec( 1. ),
            f_start_sec( 0. ),
            f_stop_sec( 1. ),
            f_interval_sec( 1. ),
            f_seed( 51385 ),
            f_length( 10 ),
            f_size( 0 ),
            f_stride( 0 )
    {
    }
    rt_gaussian_producer::~rt_gaussian_producer()
    {
    }

    void rt_gaussian_producer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );
        out_buffer< 0 >().set_name( get_name() );
        return;
    }

    void rt_gaussian_producer::execute()
    {
        count_t t_index;

        rt_data* t_out_data;
        real_t* t_current_raw;
        real_t* t_previous_raw;

        count_t t_begin = (count_t) (round( f_begin_sec / f_interval_sec ));
        count_t t_end = (count_t) (round( f_end_sec / f_interval_sec ));
        count_t t_start = (count_t) (round( f_start_sec / f_interval_sec ));
        count_t t_stop = (count_t) (round( f_stop_sec / f_interval_sec ));
        count_t t_first_unwritten_index;
        count_t t_first_requested_index;

        real_t t_amplitude = sqrt( 2. * f_impedance_ohm ) * pow( 10., (f_power_dbm - 30.) / 20. );
        random* t_random = random::get_instance();
        random_t* t_generator = t_random->allocate( f_seed );

        t_out_data = out_stream< 0 >().data();
        t_out_data->set_size( f_size );
        t_out_data->set_time_interval( f_interval_sec );
        t_out_data->set_time_index( 0 );

        out_stream< 0 >().state( stream::s_start );
        out_stream< 0 >()++;

        t_first_unwritten_index = 0;
        t_first_requested_index = t_begin;
        while( true )
        {
            if( t_first_unwritten_index >= t_end )
            {
                out_stream< 0 >().state( stream::s_stop );
                out_stream< 0 >()++;

                out_stream< 0 >().state( stream::s_exit );
                out_stream< 0 >()++;

                break;
            }

            t_out_data = out_stream< 0 >().data();
            t_out_data->set_size( f_size );
            t_out_data->set_time_interval( f_interval_sec );
            t_out_data->set_time_index( t_first_requested_index );
            t_current_raw = t_out_data->raw();

            if( t_first_unwritten_index > t_first_requested_index )
            {
                for( t_index = t_first_requested_index; t_index < t_first_unwritten_index; t_index++ )
                {
                    t_current_raw[ t_index - t_first_requested_index ] = t_previous_raw[ t_index - t_first_unwritten_index + f_size ];
                }
                for( t_index = t_first_unwritten_index; t_index < t_first_requested_index + f_size; t_index++ )
                {
                    if( (t_index >= t_start) && (t_index <= t_stop) )
                    {
                        t_current_raw[ t_index - t_first_requested_index ] = t_random->gaussian( t_generator, 0., t_amplitude );
                    }
                    else
                    {
                        t_current_raw[ t_index - t_first_requested_index ] = 0.;
                    }
                }
            }
            else
            {
                for( t_index = t_first_requested_index; t_index < t_first_requested_index + f_size; t_index++ )
                {
                    if( (t_index >= t_start) && (t_index <= t_stop) )
                    {
                        t_current_raw[ t_index - t_first_requested_index ] = t_random->gaussian( t_generator, 0., t_amplitude );
                    }
                    else
                    {
                        t_current_raw[ t_index - t_first_requested_index ] = 0.;
                    }
                }
            }

            t_first_unwritten_index = t_first_requested_index + f_size;
            t_first_requested_index = t_first_requested_index + f_stride;
            t_previous_raw = t_current_raw;

            out_stream< 0 >().state( stream::s_run );
            out_stream< 0 >()++;
        }

        return;
    }

    void rt_gaussian_producer::finalize()
    {
        out_buffer< 0 >().finalize();
        return;
    }

}
