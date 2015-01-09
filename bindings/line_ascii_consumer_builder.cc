#include "line_ascii_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_line_ascii_consumer =
        line_ascii_consumer_builder::lingual< string >( &line_ascii_consumer::set_name, "name" ) +
        line_ascii_consumer_builder::lingual< string >( &line_ascii_consumer::set_file, "file" ) +
        midge_builder::object< line_ascii_consumer >( &midge::add, "line_ascii_consumer" );

}
