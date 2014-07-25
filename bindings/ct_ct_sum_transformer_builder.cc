#include "ct_ct_sum_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_ct_ct_sum_transformer =
        ct_ct_sum_transformer_builder::lingual< string >( &ct_ct_sum_transformer::set_name, "name" ) +
        root_builder::object< ct_ct_sum_transformer >( &root::add, "ct_ct_sum_transformer" );

}
