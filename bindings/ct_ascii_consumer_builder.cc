#include "ct_ascii_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ct_ascii_consumer =
        ct_ascii_consumer_builder::lingual< string >( &ct_ascii_consumer::set_name, "name" ) +
        ct_ascii_consumer_builder::lingual< string >( &ct_ascii_consumer::set_file, "file" ) +
        midge_builder::object< ct_ascii_consumer >( &midge::add, "ct_ascii_consumer" );

}
