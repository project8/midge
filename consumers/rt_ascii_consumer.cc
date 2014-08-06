#include "rt_ascii_consumer.hh"

#include <cmath>

namespace midge
{

    rt_ascii_consumer::rt_ascii_consumer() :
            f_file( "" ),
            f_stream(),
            f_size( 0 ),
            f_interval( 1. ),
            f_in( NULL ),
            f_index( 0 )
    {
    }
    rt_ascii_consumer::~rt_ascii_consumer()
    {
    }

    bool rt_ascii_consumer::start_consumer()
    {
        f_stream.open( f_file, std::ios_base::trunc );

        f_size = in< 0 >()->get_size();
        f_interval = in< 0 >()->get_interval();
        f_in = in< 0 >()->raw();
        f_index = 0;

        return true;
    }

    bool rt_ascii_consumer::execute_consumer()
    {
        count_t t_index;

        count_t t_next = (count_t) (round( in< 0 >()->get_time() / f_interval ));

        if( t_next < f_index )
        {
            for( t_index = f_index; t_index < t_next + f_size; t_index++ )
            {
                f_stream << t_index * f_interval << " " << f_in[ t_index - t_next ] << "\n";
            }
        }
        else
        {
            for( t_index = t_next; t_index < t_next + f_size; t_index++ )
            {
                f_stream << t_index * f_interval << " " << f_in[ t_index - t_next ] << "\n";
            }
        }

        f_index = t_next + f_size;

        return true;
    }

    bool rt_ascii_consumer::stop_consumer()
    {
        f_stream.close();

        f_size = 0;
        f_interval = 1.;
        f_in = NULL;
        f_index = 0;

        return true;
    }

}
