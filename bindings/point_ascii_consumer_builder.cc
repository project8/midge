#include "point_ascii_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_point_ascii_consumer =
        point_ascii_consumer_builder::lingual< string >( &point_ascii_consumer::set_name, "name" ) +
        point_ascii_consumer_builder::lingual< string >( &point_ascii_consumer::set_file, "file" ) +
        midge_builder::object< point_ascii_consumer >( &midge::add, "point_ascii_consumer" );

}
