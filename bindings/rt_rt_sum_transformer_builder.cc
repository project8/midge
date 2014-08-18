#include "rt_rt_sum_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_rt_sum_transformer =
        rt_rt_sum_transformer_builder::lingual< string >( &rt_rt_sum_transformer::set_name, "name" ) +
        rt_rt_sum_transformer_builder::numerical< count_t >( &rt_rt_sum_transformer::set_length, "length" ) +
        root_builder::object< rt_rt_sum_transformer >( &root::add, "rt_rt_sum_transformer" );

}
