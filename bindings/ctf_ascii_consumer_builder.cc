#include "ctf_ascii_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ctf_ascii_consumer =
        ctf_ascii_consumer_builder::lingual< string >( &ctf_ascii_consumer::set_name, "name" ) +
        ctf_ascii_consumer_builder::lingual< string >( &ctf_ascii_consumer::set_file, "file" ) +
        midge_builder::object< ctf_ascii_consumer >( &midge::add, "ctf_ascii_consumer" );

}
