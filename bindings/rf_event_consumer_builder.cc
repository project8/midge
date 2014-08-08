#include "rf_event_consumer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rf_event_consumer =
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_name, "name" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_ratio_file, "ratio_file" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_background_file, "background_file" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_frequency_minimum, "frequency_minimum" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_frequency_maximum, "frequency_maximum" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_cluster_file, "cluster_file" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_cluster_add_ratio, "cluster_add_ratio" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_cluster_slope, "cluster_slope" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_cluster_spread, "cluster_spread" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_cluster_add_coefficient, "cluster_add_coefficient" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_cluster_add_power, "cluster_add_power" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_cluster_gap_coefficient, "cluster_gap_coefficient" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_cluster_gap_power, "cluster_gap_power" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_line_file, "line_file" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_line_start_score, "line_start_score" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_line_stop_score, "line_stop_score" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_line_tolerance, "line_tolerance" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_line_add_coefficient, "line_add_coefficient" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_line_add_power, "line_add_power" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_line_gap_coefficient, "line_gap_coefficient" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_line_gap_power, "line_gap_power" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_event_file, "event_file" ) +
        rf_event_consumer_builder::numerical< real_t >( &rf_event_consumer::set_event_time_tolerance, "event_time_tolerance" ) +
        rf_event_consumer_builder::boolean< bool_t >( &rf_event_consumer::set_plot, "plot" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_plot_ratio_key, "plot_ratio_key" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_plot_ratio_name, "plot_ratio_name" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_chart_ratio_title, "chart_ratio_title" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_axis_ratio_title, "axis_ratio_title" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_plot_cluster_key, "plot_cluster_key" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_plot_cluster_name, "plot_cluster_name" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_chart_cluster_title, "chart_cluster_title" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_axis_cluster_title, "axis_cluster_title" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_plot_line_key, "plot_line_key" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_plot_line_name, "plot_line_name" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_chart_line_title, "chart_line_title" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_axis_line_title, "axis_line_title" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_plot_event_key, "plot_event_key" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_plot_event_name, "plot_event_name" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_chart_event_title, "chart_event_title" ) +
        rf_event_consumer_builder::lingual< string >( &rf_event_consumer::set_axis_event_title, "axis_event_title" ) +
        root_builder::object< rf_event_consumer >( &root::add, "rf_event_consumer" );

}
