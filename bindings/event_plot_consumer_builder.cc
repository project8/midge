#include "event_plot_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_event_plot_consumer =
        event_plot_consumer_builder::lingual< string >( &event_plot_consumer::set_name, "name" ) +
        event_plot_consumer_builder::boolean< bool >( &event_plot_consumer::set_plot_undermap, "plot_undermap" ) +
        event_plot_consumer_builder::boolean< bool >( &event_plot_consumer::set_plot_overlay, "plot_overlay" ) +
        event_plot_consumer_builder::lingual< string >( &event_plot_consumer::set_plot_field, "plot_field" ) +
        event_plot_consumer_builder::lingual< string >( &event_plot_consumer::set_plot_key, "plot_key" ) +
        event_plot_consumer_builder::lingual< string >( &event_plot_consumer::set_plot_name, "plot_name" ) +
        event_plot_consumer_builder::lingual< string >( &event_plot_consumer::set_chart_title, "chart_title" ) +
        event_plot_consumer_builder::lingual< string >( &event_plot_consumer::set_x_title, "x_title" ) +
        event_plot_consumer_builder::lingual< string >( &event_plot_consumer::set_y_title, "y_title" ) +
        event_plot_consumer_builder::lingual< string >( &event_plot_consumer::set_z_title, "z_title" ) +
        midge_builder::object< event_plot_consumer >( &midge::add, "event_plot_consumer" );

}
