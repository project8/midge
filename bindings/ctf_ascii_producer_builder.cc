#include "ctf_ascii_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ctf_ascii_producer =
        ctf_ascii_producer_builder::lingual< string >( &ctf_ascii_producer::set_name, "name" ) +
        ctf_ascii_producer_builder::lingual< string >( &ctf_ascii_producer::set_file, "file" ) +
        ctf_ascii_producer_builder::numerical< count_t >( &ctf_ascii_producer::set_length, "length" ) +
        midge_builder::object< ctf_ascii_producer >( &midge::add, "ctf_ascii_producer" );

}
