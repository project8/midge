#include "rt_rt_window_transformer_builder.hh"
#include "window_rectangular_builder.hh"
#include "window_bartlett_builder.hh"
#include "window_welch_builder.hh"
#include "window_hanning_builder.hh"
#include "window_hamming_builder.hh"
#include "window_blackman_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rt_rt_window_transformer =
        rt_rt_window_transformer_builder::lingual< string >( &rt_rt_window_transformer::set_name, "name" ) +
        rt_rt_window_transformer_builder::object< window_rectangular >( &rt_rt_window_transformer::set_window, "window_rectangular" ) +
        rt_rt_window_transformer_builder::object< window_bartlett >( &rt_rt_window_transformer::set_window, "window_bartlett" ) +
        rt_rt_window_transformer_builder::object< window_welch >( &rt_rt_window_transformer::set_window, "window_welch" ) +
        rt_rt_window_transformer_builder::object< window_hanning >( &rt_rt_window_transformer::set_window, "window_hanning" ) +
        rt_rt_window_transformer_builder::object< window_hamming >( &rt_rt_window_transformer::set_window, "window_hamming" ) +
        rt_rt_window_transformer_builder::object< window_blackman >( &rt_rt_window_transformer::set_window, "window_blackman" ) +
        rt_rt_window_transformer_builder::numerical< count_t >( &rt_rt_window_transformer::set_length, "length" ) +
        midge_builder::object< rt_rt_window_transformer >( &midge::add, "rt_rt_window_transformer" );

}
