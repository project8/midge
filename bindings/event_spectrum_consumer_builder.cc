#include "event_spectrum_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_event_spectrum_consumer =
        event_spectrum_consumer_builder::lingual< string >( &event_spectrum_consumer::set_name, "name" ) +
        event_spectrum_consumer_builder::lingual< string >( &event_spectrum_consumer::set_file, "file" ) +
        event_spectrum_consumer_builder::numerical< real_t >( &event_spectrum_consumer::set_field_gauss, "field_gauss" ) +
        event_spectrum_consumer_builder::numerical< real_t >( &event_spectrum_consumer::set_frequency_mhz, "frequency_mhz" ) +
        event_spectrum_consumer_builder::numerical< real_t >( &event_spectrum_consumer::set_start_kev, "start_kev" ) +
        event_spectrum_consumer_builder::numerical< real_t >( &event_spectrum_consumer::set_stop_kev, "stop_kev" ) +
        event_spectrum_consumer_builder::numerical< real_t >( &event_spectrum_consumer::set_count, "count" ) +
        midge_builder::object< event_spectrum_consumer >( &midge::add, "event_spectrum_consumer" );

}
