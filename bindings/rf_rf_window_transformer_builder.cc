#include "rf_rf_window_transformer_builder.hh"
#include "window_rectangular_builder.hh"
#include "window_bartlett_builder.hh"
#include "window_welch_builder.hh"
#include "window_hanning_builder.hh"
#include "window_hamming_builder.hh"
#include "window_blackman_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rf_rf_window_transformer =
        rf_rf_window_transformer_builder::lingual< string >( &rf_rf_window_transformer::set_name, "name" ) +
        rf_rf_window_transformer_builder::object< window_rectangular >( &rf_rf_window_transformer::set_window, "window_rectangular" ) +
        rf_rf_window_transformer_builder::object< window_bartlett >( &rf_rf_window_transformer::set_window, "window_bartlett" ) +
        rf_rf_window_transformer_builder::object< window_welch >( &rf_rf_window_transformer::set_window, "window_welch" ) +
        rf_rf_window_transformer_builder::object< window_hanning >( &rf_rf_window_transformer::set_window, "window_hanning" ) +
        rf_rf_window_transformer_builder::object< window_hamming >( &rf_rf_window_transformer::set_window, "window_hamming" ) +
        rf_rf_window_transformer_builder::object< window_blackman >( &rf_rf_window_transformer::set_window, "window_blackman" ) +
        rf_rf_window_transformer_builder::numerical< count_t >( &rf_rf_window_transformer::set_length, "length" ) +
        midge_builder::object< rf_rf_window_transformer >( &midge::add, "rf_rf_window_transformer" );

}
