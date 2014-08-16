#include "rt_ascii_consumer.hh"

#include <cmath>

namespace midge
{

    rt_ascii_consumer::rt_ascii_consumer() :
            f_file( "" ),
            f_stream(),
            f_current( 0 )
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
        const rt_data* t_data;
        const real_t* t_raw;
        count_t t_size;
        real_t t_interval;
        real_t t_time;
        state_t t_state;
        count_t t_index;
        count_t t_next;

        while( true )
        {
            in_stream< 0 >() > t_state;
            if( t_state == stream::s_start )
            {
                f_stream.open( f_file, std::ios_base::trunc );
                f_current = 0;
                in_stream< 0 >()++;
                continue;
            }
            if( t_state == stream::s_run )
            {
                in_stream< 0 >() >> t_data;
                t_raw = t_data->raw();
                t_size = t_data->get_size();
                t_interval = t_data->get_interval();
                t_time = t_data->get_time();

                t_next = (count_t) (round( t_time / t_interval ));

                if( t_next < f_current )
                {
                    for( t_index = f_current; t_index < t_next + t_size; t_index++ )
                    {
                        f_stream << t_index * t_interval << " " << t_raw[ t_index - t_next ] << "\n";
                    }
                }
                else
                {
                    for( t_index = t_next; t_index < t_next + t_size; t_index++ )
                    {
                        f_stream << t_index * t_interval << " " << t_raw[ t_index - t_next ] << "\n";
                    }
                }

                f_current = t_next + t_size;

                in_stream< 0 >() << t_data;
                continue;
            }
            if( t_state == stream::s_stop )
            {
                f_stream.close();
                f_current = 0;
                in_stream< 0 >()++;
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
