#include "cf_data.hh"

namespace midge
{

    cf_data::cf_data() :
        f_center_time( 0. )
    {
    }
    cf_data::~cf_data()
    {
    }

    void cf_data::set_center_time( const real_t& p_time )
    {
        f_center_time = p_time;
        return;
    }
    const real_t& cf_data::get_center_time() const
    {
        return f_center_time;
    }

}
