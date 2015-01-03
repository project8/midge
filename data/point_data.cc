#include "point_data.hh"

namespace midge
{

    point_data::point_data() :
                f_data(),
                f_size( 0 ),
                f_time_interval( 1. ),
                f_time_index( 0 ),
                f_frequency_interval( 1. ),
                f_frequency_index( 0 )
    {
    }
    point_data::~point_data()
    {
    }

    point& point_data::at( const count_t& p_index )
    {
        return f_data[ p_index ];
    }
    const point& point_data::at( const count_t& p_index ) const
    {
        return f_data[ p_index ];
    }

    void point_data::set_size( const count_t& p_size )
    {
        if( f_size == p_size )
        {
            return;
        }

        f_size = p_size;
        f_data.resize( f_size );

        return;
    }
    const count_t& point_data::get_size() const
    {
        return f_size;
    }

    void point_data::set_time_interval( const real_t& p_time_interval )
    {
        f_time_interval = p_time_interval;
    }
    const real_t& point_data::get_time_interval() const
    {
        return f_time_interval;
    }

    void point_data::set_time_index( const count_t& p_time_index )
    {
        f_time_index = p_time_index;
    }
    const count_t& point_data::get_time_index() const
    {
        return f_time_index;
    }

    void point_data::set_frequency_interval( const real_t& p_frequency_interval )
    {
        f_frequency_interval = p_frequency_interval;
    }
    const real_t& point_data::get_frequency_interval() const
    {
        return f_frequency_interval;
    }

    void point_data::set_frequency_index( const count_t& p_frequency_index )
    {
        f_frequency_index = p_frequency_index;
    }
    const count_t& point_data::get_frequency_index() const
    {
        return f_frequency_index;
    }

}
