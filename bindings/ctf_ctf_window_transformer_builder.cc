#include "ctf_ctf_window_transformer_builder.hh"
#include "window_rectangular_builder.hh"
#include "window_bartlett_builder.hh"
#include "window_welch_builder.hh"
#include "window_hanning_builder.hh"
#include "window_hamming_builder.hh"
#include "window_blackman_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ctf_ctf_window_transformer =
        ctf_ctf_window_transformer_builder::lingual< string >( &ctf_ctf_window_transformer::set_name, "name" ) +
        ctf_ctf_window_transformer_builder::object< window_rectangular >( &ctf_ctf_window_transformer::set_window, "window_rectangular" ) +
        ctf_ctf_window_transformer_builder::object< window_bartlett >( &ctf_ctf_window_transformer::set_window, "window_bartlett" ) +
        ctf_ctf_window_transformer_builder::object< window_welch >( &ctf_ctf_window_transformer::set_window, "window_welch" ) +
        ctf_ctf_window_transformer_builder::object< window_hanning >( &ctf_ctf_window_transformer::set_window, "window_hanning" ) +
        ctf_ctf_window_transformer_builder::object< window_hamming >( &ctf_ctf_window_transformer::set_window, "window_hamming" ) +
        ctf_ctf_window_transformer_builder::object< window_blackman >( &ctf_ctf_window_transformer::set_window, "window_blackman" ) +
        ctf_ctf_window_transformer_builder::numerical< count_t >( &ctf_ctf_window_transformer::set_length, "length" ) +
        midge_builder::object< ctf_ctf_window_transformer >( &midge::add, "ctf_ctf_window_transformer" );

}
