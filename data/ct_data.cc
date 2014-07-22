#include "ct_data.hh"

namespace midge
{

    ct_data::ct_data() :
        f_start_time( 0. )
    {
    }
    ct_data::~ct_data()
    {
    }

    void ct_data::set_start_time( const real_t& p_time )
    {
        f_start_time = p_time;
        return;
    }
    const real_t& ct_data::get_start_time() const
    {
        return f_start_time;
    }

}
