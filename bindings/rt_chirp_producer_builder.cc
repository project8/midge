#include "rt_chirp_producer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_chirp_producer =
        rt_chirp_producer_builder::lingual< string >( &rt_chirp_producer::set_name, "name" ) +
        rt_chirp_producer_builder::numerical< real_t >( &rt_chirp_producer::set_impedance_ohm, "impedance_ohm" ) +
        rt_chirp_producer_builder::numerical< real_t >( &rt_chirp_producer::set_power_dbm, "power_dbm" ) +
        rt_chirp_producer_builder::numerical< real_t >( &rt_chirp_producer::set_start_frequency_hz, "start_frequency_hz" ) +
        rt_chirp_producer_builder::numerical< real_t >( &rt_chirp_producer::set_stop_frequency_hz, "stop_frequency_hz" ) +
        rt_chirp_producer_builder::numerical< real_t >( &rt_chirp_producer::set_phase_deg, "phase_deg" ) +
        rt_chirp_producer_builder::numerical< real_t >( &rt_chirp_producer::set_begin_sec, "begin_sec" ) +
        rt_chirp_producer_builder::numerical< real_t >( &rt_chirp_producer::set_end_sec, "end_sec" ) +
        rt_chirp_producer_builder::numerical< real_t >( &rt_chirp_producer::set_start_sec, "start_sec" ) +
        rt_chirp_producer_builder::numerical< real_t >( &rt_chirp_producer::set_stop_sec, "stop_sec" ) +
        rt_chirp_producer_builder::numerical< real_t >( &rt_chirp_producer::set_interval_sec, "interval_sec" ) +
        rt_chirp_producer_builder::numerical< count_t >( &rt_chirp_producer::set_length, "length" ) +
        rt_chirp_producer_builder::numerical< count_t >( &rt_chirp_producer::set_size, "size" ) +
        rt_chirp_producer_builder::numerical< count_t >( &rt_chirp_producer::set_stride, "stride" ) +
        root_builder::object< rt_chirp_producer >( &root::add, "rt_chirp_producer" );

}
