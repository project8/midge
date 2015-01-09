#include "line_binary_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_line_binary_producer =
        line_binary_producer_builder::lingual< string >( &line_binary_producer::set_name, "name" ) +
        line_binary_producer_builder::lingual< string >( &line_binary_producer::set_file, "file" ) +
        line_binary_producer_builder::numerical< count_t >( &line_binary_producer::set_length, "length" ) +
        midge_builder::object< line_binary_producer >( &midge::add, "line_binary_producer" );

}
