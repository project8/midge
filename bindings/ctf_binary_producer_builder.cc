#include "ctf_binary_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ctf_binary_producer =
        ctf_binary_producer_builder::lingual< string >( &ctf_binary_producer::set_name, "name" ) +
        ctf_binary_producer_builder::lingual< string >( &ctf_binary_producer::set_file, "file" ) +
        ctf_binary_producer_builder::numerical< count_t >( &ctf_binary_producer::set_length, "length" ) +
        midge_builder::object< ctf_binary_producer >( &midge::add, "ctf_binary_producer" );

}
