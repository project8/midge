#include "cf_ascii_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_cf_ascii_producer =
        cf_ascii_producer_builder::lingual< string >( &cf_ascii_producer::set_name, "name" ) +
        cf_ascii_producer_builder::lingual< string >( &cf_ascii_producer::set_file, "file" ) +
        cf_ascii_producer_builder::numerical< count_t >( &cf_ascii_producer::set_length, "length" ) +
        midge_builder::object< cf_ascii_producer >( &midge::add, "cf_ascii_producer" );

}
