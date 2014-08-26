#include "ct_binary_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ct_binary_producer =
        ct_binary_producer_builder::lingual< string >( &ct_binary_producer::set_name, "name" ) +
        ct_binary_producer_builder::lingual< string >( &ct_binary_producer::set_file, "file" ) +
        ct_binary_producer_builder::numerical< count_t >( &ct_binary_producer::set_length, "length" ) +
        midge_builder::object< ct_binary_producer >( &midge::add, "ct_binary_producer" );

}
