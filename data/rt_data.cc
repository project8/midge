#include "rt_data.hh"

namespace midge
{

    rt_data::rt_data() :
            f_raw( NULL ),
            f_size( 0 ),
            f_interval( 0. ),
            f_time( 0. )
    {
    }
    rt_data::~rt_data()
    {
        delete[] f_raw;
    }

    real_t* rt_data::raw()
    {
        return f_raw;
    }
    const real_t* rt_data::raw() const
    {
        return f_raw;
    }

    void rt_data::set_size( const count_t& p_size )
    {
        if( f_size == p_size )
        {
            return;
        }

        delete[] f_raw;
        f_size = p_size;
        f_raw = new real_t[ f_size ];

        return;
    }
    const count_t& rt_data::get_size() const
    {
        return f_size;
    }

    void rt_data::set_interval( const real_t& p_interval )
    {
        f_interval = p_interval;
        return;
    }
    const real_t& rt_data::get_interval() const
    {
        return f_interval;
    }

    void rt_data::set_time( const real_t& p_time )
    {
        f_time = p_time;
        return;
    }
    const real_t& rt_data::get_time() const
    {
        return f_time;
    }

}
