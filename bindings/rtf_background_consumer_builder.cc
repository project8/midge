#include "rtf_background_consumer_builder.hh"
#include "window_rectangular_builder.hh"
#include "window_bartlett_builder.hh"
#include "window_welch_builder.hh"
#include "window_hanning_builder.hh"
#include "window_hamming_builder.hh"
#include "window_blackman_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rtf_background_consumer =
        rtf_background_consumer_builder::lingual< string >( &rtf_background_consumer::set_name, "name" ) +
        rtf_background_consumer_builder::lingual< string >( &rtf_background_consumer::set_file, "file" ) +
        rtf_background_consumer_builder::numerical< count_t >( &rtf_background_consumer::set_length, "length" ) +
        rtf_background_consumer_builder::object< window_rectangular >( &rtf_background_consumer::set_window, "window_rectangular" ) +
        rtf_background_consumer_builder::object< window_bartlett >( &rtf_background_consumer::set_window, "window_bartlett" ) +
        rtf_background_consumer_builder::object< window_welch >( &rtf_background_consumer::set_window, "window_welch" ) +
        rtf_background_consumer_builder::object< window_hanning >( &rtf_background_consumer::set_window, "window_hanning" ) +
        rtf_background_consumer_builder::object< window_hamming >( &rtf_background_consumer::set_window, "window_hamming" ) +
        rtf_background_consumer_builder::object< window_blackman >( &rtf_background_consumer::set_window, "window_blackman" ) +
        rtf_background_consumer_builder::boolean< bool_t >( &rtf_background_consumer::set_plot, "plot" ) +
        rtf_background_consumer_builder::lingual< string >( &rtf_background_consumer::set_plot_key, "plot_key" ) +
        rtf_background_consumer_builder::lingual< string >( &rtf_background_consumer::set_plot_name, "plot_name" ) +
        rtf_background_consumer_builder::lingual< string >( &rtf_background_consumer::set_chart_title, "chart_title" ) +
        rtf_background_consumer_builder::lingual< string >( &rtf_background_consumer::set_axis_title, "axis_title" ) +
        midge_builder::object< rtf_background_consumer >( &midge::add, "rtf_background_consumer" );

}
