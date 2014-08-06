#include "rf_candidate_consumer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rf_candidate_consumer =
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_name, "name" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_threshold_file, "threshold_file" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_cluster_file, "cluster_file" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_background_file, "background_file" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_threshold, "threshold" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_frequency_minimum, "frequency_minimum" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_frequency_maximum, "frequency_maximum" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_slope, "cluster_slope" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_spread, "cluster_spread" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_add_coefficient, "cluster_add_coefficient" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_add_power, "cluster_add_power" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_gap_coefficient, "cluster_gap_coefficient" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_gap_power, "cluster_gap_power" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_score_up, "cluster_score_up" ) +
        rf_candidate_consumer_builder::numerical< real_t >( &rf_candidate_consumer::set_cluster_score_down, "cluster_score_down" ) +
        rf_candidate_consumer_builder::boolean< bool_t >( &rf_candidate_consumer::set_plot, "plot" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_plot_threshold_key, "plot_threshold_key" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_plot_threshold_name, "plot_threshold_name" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_chart_threshold_title, "chart_threshold_title" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_axis_threshold_title, "axis_threshold_title" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_plot_cluster_key, "plot_cluster_key" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_plot_cluster_name, "plot_cluster_name" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_chart_cluster_title, "chart_cluster_title" ) +
        rf_candidate_consumer_builder::lingual< string >( &rf_candidate_consumer::set_axis_cluster_title, "axis_cluster_title" ) +
        root_builder::object< rf_candidate_consumer >( &root::add, "rf_candidate_consumer" );

}
