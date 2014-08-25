#include "cf_cf_window_transformer_builder.hh"
#include "window_rectangular_builder.hh"
#include "window_bartlett_builder.hh"
#include "window_welch_builder.hh"
#include "window_hanning_builder.hh"
#include "window_hamming_builder.hh"
#include "window_blackman_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_cf_cf_window_transformer =
        cf_cf_window_transformer_builder::lingual< string >( &cf_cf_window_transformer::set_name, "name" ) +
        cf_cf_window_transformer_builder::object< window_rectangular >( &cf_cf_window_transformer::set_window, "window_rectangular" ) +
        cf_cf_window_transformer_builder::object< window_bartlett >( &cf_cf_window_transformer::set_window, "window_bartlett" ) +
        cf_cf_window_transformer_builder::object< window_welch >( &cf_cf_window_transformer::set_window, "window_welch" ) +
        cf_cf_window_transformer_builder::object< window_hanning >( &cf_cf_window_transformer::set_window, "window_hanning" ) +
        cf_cf_window_transformer_builder::object< window_hamming >( &cf_cf_window_transformer::set_window, "window_hamming" ) +
        cf_cf_window_transformer_builder::object< window_blackman >( &cf_cf_window_transformer::set_window, "window_blackman" ) +
        cf_cf_window_transformer_builder::numerical< count_t >( &cf_cf_window_transformer::set_length, "length" ) +
        midge_builder::object< cf_cf_window_transformer >( &midge::add, "cf_cf_window_transformer" );

}
