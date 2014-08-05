#include "rt_root_consumer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_root_consumer =
        rt_root_consumer_builder::lingual< string >( &rt_root_consumer::set_name, "name" ) +
        rt_root_consumer_builder::lingual< string >( &rt_root_consumer::set_file, "file" ) +
        rt_root_consumer_builder::boolean< bool_t >( &rt_root_consumer::set_plot, "plot" ) +
        rt_root_consumer_builder::lingual< string >( &rt_root_consumer::set_plot_key, "plot_key" ) +
        rt_root_consumer_builder::lingual< string >( &rt_root_consumer::set_plot_name, "plot_name" ) +
        rt_root_consumer_builder::lingual< string >( &rt_root_consumer::set_chart_title, "chart_title" ) +
        rt_root_consumer_builder::lingual< string >( &rt_root_consumer::set_axis_title, "axis_title" ) +
        root_builder::object< rt_root_consumer >( &root::add, "rt_root_consumer" );

}
