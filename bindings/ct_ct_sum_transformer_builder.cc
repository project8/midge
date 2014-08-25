#include "ct_ct_sum_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ct_ct_sum_transformer =
        ct_ct_sum_transformer_builder::lingual< string >( &ct_ct_sum_transformer::set_name, "name" ) +
        ct_ct_sum_transformer_builder::numerical< count_t >( &ct_ct_sum_transformer::set_length, "length" ) +
        midge_builder::object< ct_ct_sum_transformer >( &midge::add, "ct_ct_sum_transformer" );

}
