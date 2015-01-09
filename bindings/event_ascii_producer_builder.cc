#include "event_ascii_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_event_ascii_producer =
        event_ascii_producer_builder::lingual< string >( &event_ascii_producer::set_name, "name" ) +
        event_ascii_producer_builder::lingual< string >( &event_ascii_producer::set_file, "file" ) +
        event_ascii_producer_builder::numerical< count_t >( &event_ascii_producer::set_length, "length" ) +
        midge_builder::object< event_ascii_producer >( &midge::add, "event_ascii_producer" );

}
