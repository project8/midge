#include "line_binary_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_line_binary_consumer =
        line_binary_consumer_builder::lingual< string >( &line_binary_consumer::set_name, "name" ) +
        line_binary_consumer_builder::lingual< string >( &line_binary_consumer::set_file, "file" ) +
        midge_builder::object< line_binary_consumer >( &midge::add, "line_binary_consumer" );

}
