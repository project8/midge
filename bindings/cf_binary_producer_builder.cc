#include "cf_binary_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_cf_binary_producer =
        cf_binary_producer_builder::lingual< string >( &cf_binary_producer::set_name, "name" ) +
        cf_binary_producer_builder::lingual< string >( &cf_binary_producer::set_file, "file" ) +
        cf_binary_producer_builder::numerical< count_t >( &cf_binary_producer::set_length, "length" ) +
        midge_builder::object< cf_binary_producer >( &midge::add, "cf_binary_producer" );

}
