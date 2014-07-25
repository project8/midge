#include "rt_ct_analytic_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_ct_analytic_transformer =
        rt_ct_analytic_transformer_builder::lingual< string >( &rt_ct_analytic_transformer::set_name, "name" ) +
        root_builder::object< rt_ct_analytic_transformer >( &root::add, "rt_ct_analytic_transformer" );

}
