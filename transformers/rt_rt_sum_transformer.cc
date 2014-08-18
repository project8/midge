#include "rt_rt_sum_transformer.hh"

#include "fourier.hh"

namespace midge
{

    rt_rt_sum_transformer::rt_rt_sum_transformer() :
            f_length( 10 )
    {
    }
    rt_rt_sum_transformer::~rt_rt_sum_transformer()
    {
    }

    void rt_rt_sum_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );
        out_buffer< 0 >().set_name( get_name() );

        return;
    }

    void rt_rt_sum_transformer::execute()
    {
        count_t t_index;

        state_t t_in_zero_state;
        const rt_data* t_in_zero_data;
        real_t* t_in_zero_raw;

        state_t t_in_one_state;
        const rt_data* t_in_one_data;
        real_t* t_in_one_raw;

        rt_data* t_out_data;
        real_t* t_out_raw;

        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;

        while( true )
        {
            in_stream< 0 >()++;
            t_in_zero_state = in_stream< 0 >().state();
            t_in_zero_data = in_stream< 0 >().data();

            in_stream< 1 >()++;
            t_in_one_state = in_stream< 1 >().state();
            t_in_one_data = in_stream< 1 >().data();

            t_out_data = out_stream< 0 >().data();

            if( t_in_zero_state != t_in_one_state )
            {
                throw error() << "rt rt sum transformer <" << get_name() << "> got different commands from inputs <0> and <1>";
                return;
            }

            if( t_in_zero_state == stream::s_start )
            {
                t_size = t_in_zero_data->get_size();
                if( t_in_one_data->get_size() != t_size )
                {
                    throw error() << "rt rt sum transformer <" << get_name() << "> got data with different sizes from inputs <0> and <1>";
                    return;
                }

                t_time_interval = t_in_zero_data->get_time_interval();
                if( t_in_one_data->get_time_interval() != t_time_interval )
                {
                    throw error() << "rt rt sum transformer <" << get_name() << "> got data with different time intervals from inputs <0> and <1>";
                    return;
                }

                t_time_index = t_in_zero_data->get_time_index();
                if( t_in_one_data->get_time_index() != t_time_index )
                {
                    throw error() << "rt rt sum transformer <" << get_name() << "> got data with different time indices from inputs <0> and <1>";
                    return;
                }

                t_out_data->set_size( t_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index );

                out_stream< 0 >().state( stream::s_start );
                t_index = out_stream< 0 >()++;

                continue;
            }
            if( t_in_zero_state == stream::s_run )
            {
                t_time_index = t_in_zero_data->get_time_index();
                if( t_in_one_data->get_time_index() != t_time_index )
                {
                    throw error() << "rt rt sum transformer <" << get_name() << "> got data with different time indices from inputs <0> and <1>";
                    return;
                }

                t_in_zero_raw = t_in_zero_data->raw();
                t_in_one_raw = t_in_one_data->raw();

                t_out_data->set_size( t_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index );
                t_out_raw = t_out_data->raw();

                for( t_index = 0; t_index < t_size; t_index++ )
                {
                    t_out_raw[ t_index ] = t_in_zero_raw[ t_index ] + t_in_one_raw[ t_index ];
                }

                out_stream< 0 >().state( stream::s_run );
                t_index = out_stream< 0 >()++;

                continue;
            }
            if( t_in_zero_state == stream::s_stop )
            {
                out_stream< 0 >().state( stream::s_stop );
                t_index = out_stream< 0 >()++;

                continue;
            }
            if( t_in_zero_state == stream::s_exit )
            {
                out_stream< 0 >().state( stream::s_exit );
                t_index = out_stream< 0 >()++;

                break;
            }
        }

        return;
    }

    void rt_rt_sum_transformer::finalize()
    {
        out_buffer< 0 >().finalize();

        return;
    }

}
