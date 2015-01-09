#include "event_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_event_transformer =
        event_transformer_builder::lingual< string >( &event_transformer::set_name, "name" ) +
        event_transformer_builder::numerical< real_t >( &event_transformer::set_frequency, "frequency" ) +
        event_transformer_builder::numerical< real_t >( &event_transformer::set_time, "time" ) +
        event_transformer_builder::numerical< real_t >( &event_transformer::set_tolerance, "tolerance" ) +
        event_transformer_builder::numerical< real_t >( &event_transformer::set_delay, "delay" ) +
        event_transformer_builder::numerical< count_t >( &event_transformer::set_length, "length" ) +
        midge_builder::object< event_transformer >( &midge::add, "event_transformer" );

}
