#include "rf_average_root_consumer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rf_average_root_consumer =
        rf_average_root_consumer_builder::lingual< string >( &rf_average_root_consumer::set_name, "name" ) +
        rf_average_root_consumer_builder::lingual< string >( &rf_average_root_consumer::set_file, "file" ) +
        rf_average_root_consumer_builder::boolean< bool_t >( &rf_average_root_consumer::set_plot, "plot" ) +
        rf_average_root_consumer_builder::lingual< string >( &rf_average_root_consumer::set_plot_key, "plot_key" ) +
        rf_average_root_consumer_builder::lingual< string >( &rf_average_root_consumer::set_plot_name, "plot_name" ) +
        rf_average_root_consumer_builder::lingual< string >( &rf_average_root_consumer::set_chart_title, "chart_title" ) +
        rf_average_root_consumer_builder::lingual< string >( &rf_average_root_consumer::set_axis_title, "axis_title" ) +
        root_builder::object< rf_average_root_consumer >( &root::add, "rf_average_root_consumer" );

}
