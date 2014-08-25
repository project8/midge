#include "rt_ct_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rt_ct_transformer =
        rt_ct_transformer_builder::lingual< string >( &rt_ct_transformer::set_name, "name" ) +
        rt_ct_transformer_builder::lingual< string >( &rt_ct_transformer::set_mode, "mode" ) +
        rt_ct_transformer_builder::numerical< count_t >( &rt_ct_transformer::set_length, "length" ) +
        midge_builder::object< rt_ct_transformer >( &midge::add, "rt_ct_transformer" );

}
