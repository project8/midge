#include "event_ascii_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_event_ascii_consumer =
        event_ascii_consumer_builder::lingual< string >( &event_ascii_consumer::set_name, "name" ) +
        event_ascii_consumer_builder::lingual< string >( &event_ascii_consumer::set_file, "file" ) +
        midge_builder::object< event_ascii_consumer >( &midge::add, "event_ascii_consumer" );

}
