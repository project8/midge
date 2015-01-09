#include "point_binary_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_point_binary_producer =
        point_binary_producer_builder::lingual< string >( &point_binary_producer::set_name, "name" ) +
        point_binary_producer_builder::lingual< string >( &point_binary_producer::set_file, "file" ) +
        point_binary_producer_builder::numerical< count_t >( &point_binary_producer::set_length, "length" ) +
        midge_builder::object< point_binary_producer >( &midge::add, "point_binary_producer" );

}
