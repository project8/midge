#include "rt_data.hh"

#include "fourier.hh"

namespace midge
{

    rt_data::rt_data() :
            f_raw( NULL ),
            f_size( 0 ),
            f_time_interval( 1. ),
            f_time_index( 0 )
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
        f_size = p_size;

        if( f_raw != NULL )
        {
            fourier::get_instance()->free_real( f_raw );
        }
        f_raw = fourier::get_instance()->allocate_real( f_size );

        return;
    }
    const count_t& rt_data::get_size() const
    {
        return f_size;
    }

    void rt_data::set_time_interval( const real_t& p_time_interval )
    {
        f_time_interval = p_time_interval;
        return;
    }
    const real_t& rt_data::get_time_interval() const
    {
        return f_time_interval;
    }

    void rt_data::set_time_index( const count_t& p_time_index )
    {
        f_time_index = p_time_index;
        return;
    }
    const count_t& rt_data::get_time_index() const
    {
        return f_time_index;
    }

}
