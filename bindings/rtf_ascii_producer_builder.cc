#include "rtf_ascii_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rtf_ascii_producer =
        rtf_ascii_producer_builder::lingual< string >( &rtf_ascii_producer::set_name, "name" ) +
        rtf_ascii_producer_builder::lingual< string >( &rtf_ascii_producer::set_file, "file" ) +
        rtf_ascii_producer_builder::numerical< count_t >( &rtf_ascii_producer::set_length, "length" ) +
        midge_builder::object< rtf_ascii_producer >( &midge::add, "rtf_ascii_producer" );

}
