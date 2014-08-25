#include "cf_binary_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_cf_binary_consumer =
        cf_binary_consumer_builder::lingual< string >( &cf_binary_consumer::set_name, "name" ) +
        cf_binary_consumer_builder::lingual< string >( &cf_binary_consumer::set_file, "file" ) +
        midge_builder::object< cf_binary_consumer >( &midge::add, "cf_binary_consumer" );

}
