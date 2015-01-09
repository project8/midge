#include "line_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_line_transformer =
        line_transformer_builder::lingual< string >( &line_transformer::set_name, "name" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_threshold, "threshold" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_window, "window" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_width, "width" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_trim, "trim" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_block, "block" ) +
        line_transformer_builder::numerical< count_t >( &line_transformer::set_count, "count" ) +
        line_transformer_builder::numerical< count_t >( &line_transformer::set_score, "score" ) +
        line_transformer_builder::numerical< real_t >( &line_transformer::set_quality, "quality" ) +
        line_transformer_builder::numerical< count_t >( &line_transformer::set_length, "length" ) +
        midge_builder::object< line_transformer >( &midge::add, "line_transformer" );

}
