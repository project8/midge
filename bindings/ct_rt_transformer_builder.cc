#include "ct_rt_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_ct_rt_transformer =
        ct_rt_transformer_builder::lingual< string >( &ct_rt_transformer::set_name, "name" ) +
        ct_rt_transformer_builder::lingual< string >( &ct_rt_transformer::set_mode, "mode" ) +
        ct_rt_transformer_builder::numerical< count_t >( &ct_rt_transformer::set_length, "length" ) +
        root_builder::object< ct_rt_transformer >( &root::add, "ct_rt_transformer" );

}
