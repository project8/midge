#include "rt_ct_analytic_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rt_ct_analytic_transformer =
        rt_ct_analytic_transformer_builder::lingual< string >( &rt_ct_analytic_transformer::set_name, "name" ) +
        rt_ct_analytic_transformer_builder::numerical< count_t >( &rt_ct_analytic_transformer::set_length, "length" ) +
        midge_builder::object< rt_ct_analytic_transformer >( &midge::add, "rt_ct_analytic_transformer" );

}
