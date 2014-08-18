#include "ct_data.hh"

#include "fourier.hh"

namespace midge
{

    ct_data::ct_data() :
            f_raw( NULL ),
            f_size( 0 ),
            f_time_interval( 1. ),
            f_time_index( 0 )
    {
    }
    ct_data::~ct_data()
    {
        delete[] f_raw;
    }

    complex_t* ct_data::raw()
    {
        return f_raw;
    }
    complex_t* ct_data::raw() const
    {
        return f_raw;
    }

    void ct_data::set_size( const count_t& p_size )
    {
        if( f_size == p_size )
        {
            return;
        }
        f_size = p_size;

        if( f_raw != NULL )
        {
            fourier::get_instance()->free_complex( f_raw );
        }
        f_raw = fourier::get_instance()->allocate_complex( f_size );

        return;
    }
    const count_t& ct_data::get_size() const
    {
        return f_size;
    }

    void ct_data::set_time_interval( const real_t& p_time_interval )
    {
        f_time_interval = p_time_interval;
        return;
    }
    const real_t& ct_data::get_time_interval() const
    {
        return f_time_interval;
    }

    void ct_data::set_time_index( const count_t& p_time_index )
    {
        f_time_index = p_time_index;
        return;
    }
    const count_t& ct_data::get_time_index() const
    {
        return f_time_index;
    }

}
