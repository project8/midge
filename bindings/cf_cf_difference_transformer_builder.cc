#include "cf_cf_difference_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_cf_cf_difference_transformer =
        cf_cf_difference_transformer_builder::lingual< string >( &cf_cf_difference_transformer::set_name, "name" ) +
        cf_cf_difference_transformer_builder::numerical< count_t >( &cf_cf_difference_transformer::set_length, "length" ) +
        midge_builder::object< cf_cf_difference_transformer >( &midge::add, "cf_cf_difference_transformer" );

}
