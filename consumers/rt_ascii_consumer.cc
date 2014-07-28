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
            f_start_time( 0. ),
            f_stop_time( 0. ),
            f_current_time( 0. )
    {
    }
    rt_ascii_consumer::~rt_ascii_consumer()
    {
    }

    void rt_ascii_consumer::set_file( const string& p_file )
    {
        f_file = p_file;
        return;
    }
    const string& rt_ascii_consumer::get_file() const
    {
        return f_file;
    }

    void rt_ascii_consumer::initialize_consumer()
    {
        f_stream.open( f_file, std::ios_base::trunc );

        return;
    }

    bool rt_ascii_consumer::start_consumer()
    {
        f_size = in< 0 >()->get_size();
        f_interval = in< 0 >()->get_interval();
        f_in = in< 0 >()->raw();

        f_current_time = 0.;
        f_start_time = 0.;
        f_stop_time = f_interval * f_size;

        return true;
    }

    bool rt_ascii_consumer::execute_consumer()
    {
        f_start_time = in< 0 >()->get_start_time();
        count_t t_start_index = (count_t) (round( f_start_time / f_interval ));
        f_stop_time = in< 0 >()->get_start_time() + f_size * f_interval;
        count_t t_stop_index = (count_t) (round( f_stop_time / f_interval ));

        count_t t_current_index = f_current_time > f_start_time ? (count_t) (round( f_current_time / f_interval )) : (count_t) (round( f_start_time / f_interval));

        for( count_t t_index = t_current_index; t_index < t_stop_index; t_index++ )
        {
            f_stream << (f_start_time + (t_index - t_start_index) * f_interval) << " " << f_in[ t_index - t_start_index ] << "\n";
        }
        f_current_time = f_stop_time;

        return true;
    }

    bool rt_ascii_consumer::stop_consumer()
    {
        f_size = 0;
        f_interval = 1.;
        f_in = NULL;

        f_current_time = 0.;
        f_start_time = 0.;
        f_stop_time = 0.;

        return true;
    }

    void rt_ascii_consumer::finalize_consumer()
    {
        f_stream.close();

        return;
    }
}
