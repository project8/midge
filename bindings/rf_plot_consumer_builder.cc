#include "rf_plot_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rf_plot_consumer =
        rf_plot_consumer_builder::lingual< string >( &rf_plot_consumer::set_name, "name" ) +
        rf_plot_consumer_builder::lingual< string >( &rf_plot_consumer::set_plot_key, "plot_key" ) +
        rf_plot_consumer_builder::lingual< string >( &rf_plot_consumer::set_plot_name, "plot_name" ) +
        rf_plot_consumer_builder::lingual< string >( &rf_plot_consumer::set_chart_title, "chart_title" ) +
        rf_plot_consumer_builder::lingual< string >( &rf_plot_consumer::set_x_title, "x_title" ) +
        rf_plot_consumer_builder::lingual< string >( &rf_plot_consumer::set_y_title, "y_title" ) +
        midge_builder::object< rf_plot_consumer >( &midge::add, "rf_plot_consumer" );

}
