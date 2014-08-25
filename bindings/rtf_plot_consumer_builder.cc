#include "rtf_plot_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rtf_plot_consumer =
        rtf_plot_consumer_builder::lingual< string >( &rtf_plot_consumer::set_name, "name" ) +
        rtf_plot_consumer_builder::lingual< string >( &rtf_plot_consumer::set_plot_key, "plot_key" ) +
        rtf_plot_consumer_builder::lingual< string >( &rtf_plot_consumer::set_plot_name, "plot_name" ) +
        rtf_plot_consumer_builder::lingual< string >( &rtf_plot_consumer::set_chart_title, "chart_title" ) +
        rtf_plot_consumer_builder::lingual< string >( &rtf_plot_consumer::set_x_title, "x_title" ) +
        rtf_plot_consumer_builder::lingual< string >( &rtf_plot_consumer::set_y_title, "y_title" ) +
        rtf_plot_consumer_builder::lingual< string >( &rtf_plot_consumer::set_z_title, "z_title" ) +
        midge_builder::object< rtf_plot_consumer >( &midge::add, "rtf_plot_consumer" );

}
