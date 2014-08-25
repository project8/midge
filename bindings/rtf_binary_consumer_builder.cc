#include "rtf_binary_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rtf_binary_consumer =
        rtf_binary_consumer_builder::lingual< string >( &rtf_binary_consumer::set_name, "name" ) +
        rtf_binary_consumer_builder::lingual< string >( &rtf_binary_consumer::set_file, "file" ) +
        midge_builder::object< rtf_binary_consumer >( &midge::add, "rtf_binary_consumer" );

}
