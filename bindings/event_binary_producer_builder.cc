#include "event_binary_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_event_binary_producer =
        event_binary_producer_builder::lingual< string >( &event_binary_producer::set_name, "name" ) +
        event_binary_producer_builder::lingual< string >( &event_binary_producer::set_file, "file" ) +
        event_binary_producer_builder::numerical< count_t >( &event_binary_producer::set_length, "length" ) +
        midge_builder::object< event_binary_producer >( &midge::add, "event_binary_producer" );

}
