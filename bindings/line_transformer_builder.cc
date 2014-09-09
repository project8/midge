#include "line_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_line_transformer =
        line_transformer_builder::lingual< string >( &line_transformer::set_name, "name" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_ratio_seed, "ratio_seed" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_ratio_low, "ratio_low" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_ratio_high, "ratio_high" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_window_seed, "window_seed" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_window_low, "window_low" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_window_high, "window_high" ) +
        line_transformer_builder::numerical< count_t >( &line_transformer::set_count, "count" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_tolerance, "tolerance" ) +
        line_transformer_builder::numerical< count_t >( &line_transformer::set_length, "length" ) +
        midge_builder::object< line_transformer >( &midge::add, "line_transformer" );

}
