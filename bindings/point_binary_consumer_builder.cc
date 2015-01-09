#include "point_binary_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_point_binary_consumer =
        point_binary_consumer_builder::lingual< string >( &point_binary_consumer::set_name, "name" ) +
        point_binary_consumer_builder::lingual< string >( &point_binary_consumer::set_file, "file" ) +
        midge_builder::object< point_binary_consumer >( &midge::add, "point_binary_consumer" );

}
