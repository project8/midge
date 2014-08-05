#include "rt_monarch_producer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_monarch_producer =
        rt_monarch_producer_builder::lingual< string >( &rt_monarch_producer::set_name, "name" ) +
        rt_monarch_producer_builder::lingual< string >( &rt_monarch_producer::set_file, "file" ) +
        rt_monarch_producer_builder::numerical< real_t >( &rt_monarch_producer::set_minimum_time, "minimum_time" ) +
        rt_monarch_producer_builder::numerical< real_t >( &rt_monarch_producer::set_maximum_time, "maximum_time" ) +
        rt_monarch_producer_builder::numerical< count_t >( &rt_monarch_producer::set_stride, "stride" ) +
        rt_monarch_producer_builder::numerical< count_t >( &rt_monarch_producer::set_size, "size" ) +
        root_builder::object< rt_monarch_producer >( &root::add, "rt_monarch_producer" );

}
