#include "cf_ascii_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_cf_ascii_consumer =
        cf_ascii_consumer_builder::lingual< string >( &cf_ascii_consumer::set_name, "name" ) +
        cf_ascii_consumer_builder::lingual< string >( &cf_ascii_consumer::set_file, "file" ) +
        midge_builder::object< cf_ascii_consumer >( &midge::add, "cf_ascii_consumer" );

}
