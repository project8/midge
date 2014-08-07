#include "rf_background_consumer_builder.hh"
#include "window_rectangular_builder.hh"
#include "window_bartlett_builder.hh"
#include "window_welch_builder.hh"
#include "window_hanning_builder.hh"
#include "window_hamming_builder.hh"
#include "window_blackman_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rf_background_consumer =
        rf_background_consumer_builder::lingual< string >( &rf_background_consumer::set_name, "name" ) +
        rf_background_consumer_builder::lingual< string >( &rf_background_consumer::set_file, "file" ) +
        rf_background_consumer_builder::numerical< count_t >( &rf_background_consumer::set_length, "length" ) +
        rf_background_consumer_builder::object< window_rectangular >( &rf_background_consumer::set_window, "window_rectangular" ) +
        rf_background_consumer_builder::object< window_bartlett >( &rf_background_consumer::set_window, "window_bartlett" ) +
        rf_background_consumer_builder::object< window_welch >( &rf_background_consumer::set_window, "window_welch" ) +
        rf_background_consumer_builder::object< window_hanning >( &rf_background_consumer::set_window, "window_hanning" ) +
        rf_background_consumer_builder::object< window_hamming >( &rf_background_consumer::set_window, "window_hamming" ) +
        rf_background_consumer_builder::object< window_blackman >( &rf_background_consumer::set_window, "window_blackman" ) +
        rf_background_consumer_builder::boolean< bool_t >( &rf_background_consumer::set_plot, "plot" ) +
        rf_background_consumer_builder::lingual< string >( &rf_background_consumer::set_plot_key, "plot_key" ) +
        rf_background_consumer_builder::lingual< string >( &rf_background_consumer::set_plot_name, "plot_name" ) +
        rf_background_consumer_builder::lingual< string >( &rf_background_consumer::set_chart_title, "chart_title" ) +
        rf_background_consumer_builder::lingual< string >( &rf_background_consumer::set_axis_title, "axis_title" ) +
        root_builder::object< rf_background_consumer >( &root::add, "rf_background_consumer" );

}
