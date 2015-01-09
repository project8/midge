#include "point_ascii_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_point_ascii_producer =
        point_ascii_producer_builder::lingual< string >( &point_ascii_producer::set_name, "name" ) +
        point_ascii_producer_builder::lingual< string >( &point_ascii_producer::set_file, "file" ) +
        point_ascii_producer_builder::numerical< count_t >( &point_ascii_producer::set_length, "length" ) +
        midge_builder::object< point_ascii_producer >( &midge::add, "point_ascii_producer" );

}
