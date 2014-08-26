#include "rt_binary_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rt_binary_producer =
        rt_binary_producer_builder::lingual< string >( &rt_binary_producer::set_name, "name" ) +
        rt_binary_producer_builder::lingual< string >( &rt_binary_producer::set_file, "file" ) +
        rt_binary_producer_builder::numerical< count_t >( &rt_binary_producer::set_length, "length" ) +
        midge_builder::object< rt_binary_producer >( &midge::add, "rt_binary_producer" );

}
