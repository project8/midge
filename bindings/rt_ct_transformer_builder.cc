#include "rt_ct_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_ct_transformer =
        rt_ct_transformer_builder::lingual< string >( &rt_ct_transformer::set_name, "name" ) +
        rt_ct_transformer_builder::lingual< string >( &rt_ct_transformer::set_mode, "mode" ) +
        root_builder::object< rt_ct_transformer >( &root::add, "rt_ct_transformer" );

}
