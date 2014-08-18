#include "rt_plot_consumer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_plot_consumer =
        rt_plot_consumer_builder::lingual< string >( &rt_plot_consumer::set_name, "name" ) +
        rt_plot_consumer_builder::lingual< string >( &rt_plot_consumer::set_plot_key, "plot_key" ) +
        rt_plot_consumer_builder::lingual< string >( &rt_plot_consumer::set_plot_name, "plot_name" ) +
        rt_plot_consumer_builder::lingual< string >( &rt_plot_consumer::set_chart_title, "chart_title" ) +
        rt_plot_consumer_builder::lingual< string >( &rt_plot_consumer::set_x_title, "x_title" ) +
        rt_plot_consumer_builder::lingual< string >( &rt_plot_consumer::set_y_title, "y_title" ) +
        root_builder::object< rt_plot_consumer >( &root::add, "rt_plot_consumer" );

}
