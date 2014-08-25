#include "rt_sinusoidal_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rt_sinusoidal_producer =
        rt_sinusoidal_producer_builder::lingual< string >( &rt_sinusoidal_producer::set_name, "name" ) +
        rt_sinusoidal_producer_builder::numerical< real_t >( &rt_sinusoidal_producer::set_impedance_ohm, "impedance_ohm" ) +
        rt_sinusoidal_producer_builder::numerical< real_t >( &rt_sinusoidal_producer::set_carrier_power_dbm, "carrier_power_dbm" ) +
        rt_sinusoidal_producer_builder::numerical< real_t >( &rt_sinusoidal_producer::set_carrier_frequency_hz, "carrier_frequency_hz" ) +
        rt_sinusoidal_producer_builder::numerical< real_t >( &rt_sinusoidal_producer::set_carrier_phase_deg, "carrier_phase_deg" ) +
        rt_sinusoidal_producer_builder::numerical< real_t >( &rt_sinusoidal_producer::set_signal_amplitude_hz, "signal_amplitude_hz" ) +
        rt_sinusoidal_producer_builder::numerical< real_t >( &rt_sinusoidal_producer::set_signal_frequency_hz, "signal_frequency_hz" ) +
        rt_sinusoidal_producer_builder::numerical< real_t >( &rt_sinusoidal_producer::set_signal_phase_deg, "signal_phase_deg" ) +
        rt_sinusoidal_producer_builder::numerical< real_t >( &rt_sinusoidal_producer::set_begin_sec, "begin_sec" ) +
        rt_sinusoidal_producer_builder::numerical< real_t >( &rt_sinusoidal_producer::set_end_sec, "end_sec" ) +
        rt_sinusoidal_producer_builder::numerical< real_t >( &rt_sinusoidal_producer::set_start_sec, "start_sec" ) +
        rt_sinusoidal_producer_builder::numerical< real_t >( &rt_sinusoidal_producer::set_stop_sec, "stop_sec" ) +
        rt_sinusoidal_producer_builder::numerical< real_t >( &rt_sinusoidal_producer::set_interval_sec, "interval_sec" ) +
        rt_sinusoidal_producer_builder::numerical< count_t >( &rt_sinusoidal_producer::set_length, "length" ) +
        rt_sinusoidal_producer_builder::numerical< count_t >( &rt_sinusoidal_producer::set_size, "size" ) +
        rt_sinusoidal_producer_builder::numerical< count_t >( &rt_sinusoidal_producer::set_stride, "stride" ) +
        midge_builder::object< rt_sinusoidal_producer >( &midge::add, "rt_sinusoidal_producer" );

}
