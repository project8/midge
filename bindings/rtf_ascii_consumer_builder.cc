#include "rtf_ascii_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rtf_ascii_consumer =
        rtf_ascii_consumer_builder::lingual< string >( &rtf_ascii_consumer::set_name, "name" ) +
        rtf_ascii_consumer_builder::lingual< string >( &rtf_ascii_consumer::set_file, "file" ) +
        midge_builder::object< rtf_ascii_consumer >( &midge::add, "rtf_ascii_consumer" );

}
