#include "line_ascii_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_line_ascii_producer =
        line_ascii_producer_builder::lingual< string >( &line_ascii_producer::set_name, "name" ) +
        line_ascii_producer_builder::lingual< string >( &line_ascii_producer::set_file, "file" ) +
        line_ascii_producer_builder::numerical< count_t >( &line_ascii_producer::set_length, "length" ) +
        midge_builder::object< line_ascii_producer >( &midge::add, "line_ascii_producer" );

}
