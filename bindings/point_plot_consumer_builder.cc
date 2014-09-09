#include "point_plot_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_point_plot_consumer =
        point_plot_consumer_builder::lingual< string >( &point_plot_consumer::set_name, "name" ) +
        point_plot_consumer_builder::lingual< string >( &point_plot_consumer::set_plot_field, "plot_field" ) +
        point_plot_consumer_builder::lingual< string >( &point_plot_consumer::set_plot_key, "plot_key" ) +
        point_plot_consumer_builder::lingual< string >( &point_plot_consumer::set_plot_name, "plot_name" ) +
        point_plot_consumer_builder::lingual< string >( &point_plot_consumer::set_chart_title, "chart_title" ) +
        point_plot_consumer_builder::lingual< string >( &point_plot_consumer::set_x_title, "x_title" ) +
        point_plot_consumer_builder::lingual< string >( &point_plot_consumer::set_y_title, "y_title" ) +
        point_plot_consumer_builder::lingual< string >( &point_plot_consumer::set_z_title, "z_title" ) +
        midge_builder::object< point_plot_consumer >( &midge::add, "point_plot_consumer" );

}
