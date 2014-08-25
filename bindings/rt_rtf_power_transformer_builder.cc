#include "rt_rtf_power_transformer_builder.hh"
#include "window_rectangular_builder.hh"
#include "window_bartlett_builder.hh"
#include "window_welch_builder.hh"
#include "window_hanning_builder.hh"
#include "window_hamming_builder.hh"
#include "window_blackman_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rt_rtf_power_transformer =
        rt_rtf_power_transformer_builder::lingual< string >( &rt_rtf_power_transformer::set_name, "name" ) +
        rt_rtf_power_transformer_builder::numerical< count_t >( &rt_rtf_power_transformer::set_length, "length" ) +
        rt_rtf_power_transformer_builder::numerical< real_t >( &rt_rtf_power_transformer::set_impedance_ohm, "impedance_ohm" ) +
        rt_rtf_power_transformer_builder::object< window_rectangular >( &rt_rtf_power_transformer::set_window, "window_rectangular" ) +
        rt_rtf_power_transformer_builder::object< window_bartlett >( &rt_rtf_power_transformer::set_window, "window_bartlett" ) +
        rt_rtf_power_transformer_builder::object< window_welch >( &rt_rtf_power_transformer::set_window, "window_welch" ) +
        rt_rtf_power_transformer_builder::object< window_hanning >( &rt_rtf_power_transformer::set_window, "window_hanning" ) +
        rt_rtf_power_transformer_builder::object< window_hamming >( &rt_rtf_power_transformer::set_window, "window_hamming" ) +
        rt_rtf_power_transformer_builder::object< window_blackman >( &rt_rtf_power_transformer::set_window, "window_blackman" ) +
        midge_builder::object< rt_rtf_power_transformer >( &midge::add, "rt_rtf_power_transformer" );

}
