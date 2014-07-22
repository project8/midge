#include "rt_data.hh"

namespace midge
{

    rt_data::rt_data() :
        f_start_time( 0. )
    {
    }
    rt_data::~rt_data()
    {
    }

    void rt_data::set_start_time( const real_t& p_time )
    {
        f_start_time = p_time;
        return;
    }
    const real_t& rt_data::get_start_time() const
    {
        return f_start_time;
    }

}
