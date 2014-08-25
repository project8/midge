#include "ct_ct_window_transformer_builder.hh"
#include "window_rectangular_builder.hh"
#include "window_bartlett_builder.hh"
#include "window_welch_builder.hh"
#include "window_hanning_builder.hh"
#include "window_hamming_builder.hh"
#include "window_blackman_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ct_ct_window_transformer =
        ct_ct_window_transformer_builder::lingual< string >( &ct_ct_window_transformer::set_name, "name" ) +
        ct_ct_window_transformer_builder::object< window_rectangular >( &ct_ct_window_transformer::set_window, "window_rectangular" ) +
        ct_ct_window_transformer_builder::object< window_bartlett >( &ct_ct_window_transformer::set_window, "window_bartlett" ) +
        ct_ct_window_transformer_builder::object< window_welch >( &ct_ct_window_transformer::set_window, "window_welch" ) +
        ct_ct_window_transformer_builder::object< window_hanning >( &ct_ct_window_transformer::set_window, "window_hanning" ) +
        ct_ct_window_transformer_builder::object< window_hamming >( &ct_ct_window_transformer::set_window, "window_hamming" ) +
        ct_ct_window_transformer_builder::object< window_blackman >( &ct_ct_window_transformer::set_window, "window_blackman" ) +
        ct_ct_window_transformer_builder::numerical< count_t >( &ct_ct_window_transformer::set_length, "length" ) +
        midge_builder::object< ct_ct_window_transformer >( &midge::add, "ct_ct_window_transformer" );

}
