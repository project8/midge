#include "line_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_line_transformer =
        line_transformer_builder::lingual< string >( &line_transformer::set_name, "name" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_threshold, "threshold" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_window, "window" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_tolerance, "tolerance" ) +
        line_transformer_builder::numerical< count_t >( &line_transformer::set_count, "count" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_hit_factor, "hit_factor" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_hit_power, "hit_power" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_miss_factor, "miss_factor" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_miss_power, "miss_power" ) +
        line_transformer_builder::numerical< count_t >( &line_transformer::set_score_low, "score_low" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_score_high, "score_high" ) +
        line_transformer_builder::numerical< count_t >( &line_transformer::set_length, "length" ) +
        midge_builder::object< line_transformer >( &midge::add, "line_transformer" );

}
