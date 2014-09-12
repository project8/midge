#include "line_plot_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_line_plot_consumer =
        line_plot_consumer_builder::lingual< string >( &line_plot_consumer::set_name, "name" ) +
        line_plot_consumer_builder::boolean< bool >( &line_plot_consumer::set_plot_overlay, "plot_overlay" ) +
        line_plot_consumer_builder::lingual< string >( &line_plot_consumer::set_plot_field, "plot_field" ) +
        line_plot_consumer_builder::lingual< string >( &line_plot_consumer::set_plot_key, "plot_key" ) +
        line_plot_consumer_builder::lingual< string >( &line_plot_consumer::set_plot_name, "plot_name" ) +
        line_plot_consumer_builder::lingual< string >( &line_plot_consumer::set_chart_title, "chart_title" ) +
        line_plot_consumer_builder::lingual< string >( &line_plot_consumer::set_x_title, "x_title" ) +
        line_plot_consumer_builder::lingual< string >( &line_plot_consumer::set_y_title, "y_title" ) +
        line_plot_consumer_builder::lingual< string >( &line_plot_consumer::set_z_title, "z_title" ) +
        midge_builder::object< line_plot_consumer >( &midge::add, "line_plot_consumer" );

}
