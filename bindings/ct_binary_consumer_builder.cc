#include "ct_binary_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ct_binary_consumer =
        ct_binary_consumer_builder::lingual< string >( &ct_binary_consumer::set_name, "name" ) +
        ct_binary_consumer_builder::lingual< string >( &ct_binary_consumer::set_file, "file" ) +
        midge_builder::object< ct_binary_consumer >( &midge::add, "ct_binary_consumer" );

}
