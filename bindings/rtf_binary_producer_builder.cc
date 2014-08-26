#include "rtf_binary_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rtf_binary_producer =
        rtf_binary_producer_builder::lingual< string >( &rtf_binary_producer::set_name, "name" ) +
        rtf_binary_producer_builder::lingual< string >( &rtf_binary_producer::set_file, "file" ) +
        rtf_binary_producer_builder::numerical< count_t >( &rtf_binary_producer::set_length, "length" ) +
        midge_builder::object< rtf_binary_producer >( &midge::add, "rtf_binary_producer" );

}
