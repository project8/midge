#include "point_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_point_transformer =
        point_transformer_builder::lingual< string >( &point_transformer::set_name, "name" ) +
        point_transformer_builder::numerical< real_t >( &point_transformer::set_begin_sec, "begin_sec" ) +
        point_transformer_builder::numerical< real_t >( &point_transformer::set_end_sec, "end_sec" ) +
        point_transformer_builder::numerical< real_t >( &point_transformer::set_begin_hz, "begin_hz" ) +
        point_transformer_builder::numerical< real_t >( &point_transformer::set_end_hz, "end_hz" ) +
        point_transformer_builder::numerical< count_t >( &point_transformer::set_length, "length" ) +
        midge_builder::object< point_transformer >( &midge::add, "point_transformer" );

}
