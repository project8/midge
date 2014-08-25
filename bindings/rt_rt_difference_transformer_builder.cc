#include "rt_rt_difference_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rt_rt_difference_transformer =
        rt_rt_difference_transformer_builder::lingual< string >( &rt_rt_difference_transformer::set_name, "name" ) +
        rt_rt_difference_transformer_builder::numerical< count_t >( &rt_rt_difference_transformer::set_length, "length" ) +
        midge_builder::object< rt_rt_difference_transformer >( &midge::add, "rt_rt_difference_transformer" );

}
