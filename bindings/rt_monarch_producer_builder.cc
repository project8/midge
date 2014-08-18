#include "rt_monarch_producer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_monarch_producer =
        rt_monarch_producer_builder::lingual< string >( &rt_monarch_producer::set_name, "name" ) +
        rt_monarch_producer_builder::lingual< string >( &rt_monarch_producer::set_file, "file" ) +
        rt_monarch_producer_builder::numerical< real_t >( &rt_monarch_producer::set_begin_sec, "begin_sec" ) +
        rt_monarch_producer_builder::numerical< real_t >( &rt_monarch_producer::set_end_sec, "end_sec" ) +
        rt_monarch_producer_builder::numerical< count_t >( &rt_monarch_producer::set_size, "size" ) +
        rt_monarch_producer_builder::numerical< count_t >( &rt_monarch_producer::set_stride, "stride" ) +
        rt_monarch_producer_builder::numerical< count_t >( &rt_monarch_producer::set_length, "length" ) +
        root_builder::object< rt_monarch_producer >( &root::add, "rt_monarch_producer" );

}
