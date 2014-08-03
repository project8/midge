#include "rt_data_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_data =
        rt_data_builder::lingual< string >( &rt_data::set_name, "name" ) +
        rt_data_builder::numerical< count_t >( &rt_data::set_size, "size" ) +
        rt_data_builder::numerical< real_t >( &rt_data::set_interval, "interval" ) +
        rt_data_builder::numerical< real_t >( &rt_data::set_time, "time" ) +
        root_builder::object< rt_data >( &root::add, "rt_data" );

}
