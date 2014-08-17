#include "rt_ascii_consumer.hh"

#include <cmath>

namespace midge
{

    rt_ascii_consumer::rt_ascii_consumer() :
            f_file( "" )
    {
    }
    rt_ascii_consumer::~rt_ascii_consumer()
    {
    }

    void rt_ascii_consumer::initialize()
    {
        return;
    }

    void rt_ascii_consumer::execute()
    {
        count_t t_index;

        state_t t_state;
        const rt_data* t_in_data;
        const real_t* t_in_raw;
        count_t t_in_size;
        real_t t_in_time_interval;
        count_t t_in_time_index;

        ofstream t_stream;
        count_t t_last;

        while( true )
        {
            in_stream< 0 >()++;
            t_state = in_stream< 0 >().state();
            t_in_data = in_stream< 0 >().data();

            if( t_state == stream::s_start )
            {
                t_stream.open( f_file, std::ios_base::trunc );
                t_last = 0;
                continue;
            }
            if( t_state == stream::s_run )
            {
                t_in_raw = t_in_data->raw();
                t_in_size = t_in_data->get_size();
                t_in_time_interval = t_in_data->get_time_interval();
                t_in_time_index = t_in_data->get_time_index();

                if( t_in_time_index < t_last )
                {
                    for( t_index = t_last; t_index < t_in_time_index + t_in_size; t_index++ )
                    {
                        t_stream << t_index * t_in_time_interval << " " << t_in_raw[ t_index - t_in_time_index ] << "\n";
                    }
                }
                else
                {
                    for( t_index = t_in_time_index; t_index < t_in_time_index + t_in_size; t_index++ )
                    {
                        t_stream << t_index * t_in_time_interval << " " << t_in_raw[ t_index - t_in_time_index ] << "\n";
                    }
                }

                t_last = t_in_time_index + t_in_size;

                continue;
            }
            if( t_state == stream::s_stop )
            {
                t_stream.close();
                t_last = 0;
                continue;
            }
            if( t_state == stream::s_exit )
            {
                break;
            }
        }

        return;
    }

    void rt_ascii_consumer::finalize()
    {
        return;
    }

}
