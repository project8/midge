#include "rf_data.hh"

#include "core_message.hh"

namespace midge
{

    rf_data::rf_data() :
            f_raw( NULL ),
            f_size( 0 ),
            f_time_interval( 1. ),
            f_time_index( 0 ),
            f_frequency_interval( 1. ),
            f_frequency_index( 0 )
    {
    }
    rf_data::~rf_data()
    {
        delete[] f_raw;
    }

    real_t* rf_data::raw()
    {
        return f_raw;
    }
    const real_t* rf_data::raw() const
    {
        return f_raw;
    }

    void rf_data::set_size( const count_t& p_size )
    {
        if( f_size == p_size )
        {
            return;
        }

        f_size = p_size;
        delete[] f_raw;
        f_raw = new real_t[ f_size ];

        return;
    }
    const count_t& rf_data::get_size() const
    {
        return f_size;
    }

    void rf_data::set_time_interval( const real_t& p_time_interval )
    {
        f_time_interval = p_time_interval;
        return;
    }
    const real_t& rf_data::get_time_interval() const
    {
        return f_time_interval;
    }

    void rf_data::set_time_index( const count_t& p_time_index )
    {
        f_time_index = p_time_index;
        return;
    }
    const count_t& rf_data::get_time_index() const
    {
        return f_time_index;
    }

    void rf_data::set_frequency_interval( const real_t& p_frequency_interval )
    {
        f_frequency_interval = p_frequency_interval;
        return;
    }
    const real_t& rf_data::get_frequency_interval() const
    {
        return f_frequency_interval;
    }

    void rf_data::set_frequency_index( const count_t& p_frequency_index )
    {
        f_frequency_index = p_frequency_index;
        return;
    }
    const count_t& rf_data::get_frequency_index() const
    {
        return f_frequency_index;
    }

}
