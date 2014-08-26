#include "ct_ascii_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ct_ascii_producer =
        ct_ascii_producer_builder::lingual< string >( &ct_ascii_producer::set_name, "name" ) +
        ct_ascii_producer_builder::lingual< string >( &ct_ascii_producer::set_file, "file" ) +
        ct_ascii_producer_builder::numerical< count_t >( &ct_ascii_producer::set_length, "length" ) +
        midge_builder::object< ct_ascii_producer >( &midge::add, "ct_ascii_producer" );

}
