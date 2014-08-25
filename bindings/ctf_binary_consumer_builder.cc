#include "ctf_binary_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ctf_binary_consumer =
        ctf_binary_consumer_builder::lingual< string >( &ctf_binary_consumer::set_name, "name" ) +
        ctf_binary_consumer_builder::lingual< string >( &ctf_binary_consumer::set_file, "file" ) +
        midge_builder::object< ctf_binary_consumer >( &midge::add, "ctf_binary_consumer" );

}
