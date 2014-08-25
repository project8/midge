#include "rtf_root_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rtf_root_consumer =
        rtf_root_consumer_builder::lingual< string >( &rtf_root_consumer::set_name, "name" ) +
        rtf_root_consumer_builder::lingual< string >( &rtf_root_consumer::set_file, "file" ) +
        midge_builder::object< rtf_root_consumer >( &midge::add, "rtf_root_consumer" );

}
