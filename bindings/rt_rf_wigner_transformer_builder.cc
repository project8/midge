#include "rt_rf_wigner_transformer_builder.hh"
#include "window_rectangular_builder.hh"
#include "window_bartlett_builder.hh"
#include "window_welch_builder.hh"
#include "window_hanning_builder.hh"
#include "window_hamming_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_rf_wigner_transformer =
        rt_rf_wigner_transformer_builder::lingual< string >( &rt_rf_wigner_transformer::set_name, "name" ) +
        rt_rf_wigner_transformer_builder::numerical< count_t >( &rt_rf_wigner_transformer::set_offset, "offset" ) +
        rt_rf_wigner_transformer_builder::numerical< count_t >( &rt_rf_wigner_transformer::set_length, "length" ) +
        rt_rf_wigner_transformer_builder::object< window_rectangular >( &rt_rf_wigner_transformer::set_window, "window_rectangular" ) +
        rt_rf_wigner_transformer_builder::object< window_bartlett >( &rt_rf_wigner_transformer::set_window, "window_bartlett" ) +
        rt_rf_wigner_transformer_builder::object< window_welch >( &rt_rf_wigner_transformer::set_window, "window_welch" ) +
        rt_rf_wigner_transformer_builder::object< window_hanning >( &rt_rf_wigner_transformer::set_window, "window_hanning" ) +
        rt_rf_wigner_transformer_builder::object< window_hamming >( &rt_rf_wigner_transformer::set_window, "window_hamming" ) +
        root_builder::object< rt_rf_wigner_transformer >( &root::add, "rt_rf_wigner_transformer" );

}