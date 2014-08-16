#include "rt_harmonic_producer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_harmonic_producer =
        rt_harmonic_producer_builder::lingual< string >( &rt_harmonic_producer::set_name, "name" ) +
        rt_harmonic_producer_builder::numerical< real_t >( &rt_harmonic_producer::set_power_dbm, "power_dbm" ) +
        rt_harmonic_producer_builder::numerical< real_t >( &rt_harmonic_producer::set_impedance_ohm, "impedance_ohm" ) +
        rt_harmonic_producer_builder::numerical< real_t >( &rt_harmonic_producer::set_frequency_hz, "frequency_hz" ) +
        rt_harmonic_producer_builder::numerical< real_t >( &rt_harmonic_producer::set_phase_deg, "phase_deg" ) +
        rt_harmonic_producer_builder::numerical< real_t >( &rt_harmonic_producer::set_start_sec, "start_sec" ) +
        rt_harmonic_producer_builder::numerical< real_t >( &rt_harmonic_producer::set_stop_sec, "stop_sec" ) +
        rt_harmonic_producer_builder::numerical< real_t >( &rt_harmonic_producer::set_interval_sec, "interval_sec" ) +
        rt_harmonic_producer_builder::numerical< count_t >( &rt_harmonic_producer::set_length, "length" ) +
        rt_harmonic_producer_builder::numerical< count_t >( &rt_harmonic_producer::set_size, "size" ) +
        rt_harmonic_producer_builder::numerical< count_t >( &rt_harmonic_producer::set_stride, "stride" ) +
        root_builder::object< rt_harmonic_producer >( &root::add, "rt_harmonic_producer" );

}
