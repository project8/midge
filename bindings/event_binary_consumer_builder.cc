#include "event_binary_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_event_binary_consumer =
        event_binary_consumer_builder::lingual< string >( &event_binary_consumer::set_name, "name" ) +
        event_binary_consumer_builder::lingual< string >( &event_binary_consumer::set_file, "file" ) +
        midge_builder::object< event_binary_consumer >( &midge::add, "event_binary_consumer" );

}
