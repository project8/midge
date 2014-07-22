#include "rf_data.hh"

namespace midge
{

    rf_data::rf_data() :
        f_center_time( 0. )
    {
    }
    rf_data::~rf_data()
    {
    }

    void rf_data::set_center_time( const real_t& p_time )
    {
        f_center_time = p_time;
        return;
    }
    const real_t& rf_data::get_center_time() const
    {
        return f_center_time;
    }

}
