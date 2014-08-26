#include "rtf_rf_background_transformer_builder.hh"
#include "window_rectangular_builder.hh"
#include "window_bartlett_builder.hh"
#include "window_welch_builder.hh"
#include "window_hanning_builder.hh"
#include "window_hamming_builder.hh"
#include "window_blackman_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rtf_rf_background_transformer =
        rtf_rf_background_transformer_builder::lingual< string >( &rtf_rf_background_transformer::set_name, "name" ) +
        rtf_rf_background_transformer_builder::numerical< count_t >( &rtf_rf_background_transformer::set_width, "width" ) +
        rtf_rf_background_transformer_builder::object< window_rectangular >( &rtf_rf_background_transformer::set_window, "window_rectangular" ) +
        rtf_rf_background_transformer_builder::object< window_bartlett >( &rtf_rf_background_transformer::set_window, "window_bartlett" ) +
        rtf_rf_background_transformer_builder::object< window_welch >( &rtf_rf_background_transformer::set_window, "window_welch" ) +
        rtf_rf_background_transformer_builder::object< window_hanning >( &rtf_rf_background_transformer::set_window, "window_hanning" ) +
        rtf_rf_background_transformer_builder::object< window_hamming >( &rtf_rf_background_transformer::set_window, "window_hamming" ) +
        rtf_rf_background_transformer_builder::object< window_blackman >( &rtf_rf_background_transformer::set_window, "window_blackman" ) +
        rtf_rf_background_transformer_builder::numerical< count_t >( &rtf_rf_background_transformer::set_length, "length" ) +
        midge_builder::object< rtf_rf_background_transformer >( &midge::add, "rtf_rf_background_transformer" );

}
