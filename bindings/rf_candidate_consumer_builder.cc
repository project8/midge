#include "rf_candidate_consumer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rf_candidate_consumer =
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_name, "name" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_ratio_file, "ratio_file" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_background_file, "background_file" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_frequency_minimum, "frequency_minimum" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_frequency_maximum, "frequency_maximum" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_cluster_file, "cluster_file" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_threshold, "cluster_threshold" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_slope, "cluster_slope" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_spread, "cluster_spread" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_add_coefficient, "cluster_add_coefficient" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_add_power, "cluster_add_power" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_gap_coefficient, "cluster_gap_coefficient" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_gap_power, "cluster_gap_power" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_line_file, "line_file" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_line_threshold, "line_threshold" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_line_tolerance, "line_tolerance" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_line_add_coefficient, "line_add_coefficient" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_line_add_power, "line_add_power" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_line_gap_coefficient, "line_gap_coefficient" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_line_gap_power, "line_gap_power" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_event_file, "event_file" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_event_threshold, "event_threshold" ) +
        rf_candidate_consumer_builder::boolean< bool_t >( &rf_candidate_consumer::set_plot, "plot" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_plot_ratio_key, "plot_ratio_key" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_plot_ratio_name, "plot_ratio_name" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_chart_ratio_title, "chart_ratio_title" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_axis_ratio_title, "axis_ratio_title" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_plot_cluster_key, "plot_cluster_key" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_plot_cluster_name, "plot_cluster_name" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_chart_cluster_title, "chart_cluster_title" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_axis_cluster_title, "axis_cluster_title" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_plot_line_key, "plot_line_key" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_plot_line_name, "plot_line_name" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_chart_line_title, "chart_line_title" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_axis_line_title, "axis_line_title" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_plot_event_key, "plot_event_key" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_plot_event_name, "plot_event_name" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_chart_event_title, "chart_event_title" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_axis_event_title, "axis_event_title" ) +
        root_builder::object< rf_candidate_consumer >( &root::add, "rf_candidate_consumer" );

}
