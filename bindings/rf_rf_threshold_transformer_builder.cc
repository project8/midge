#include "rf_rf_threshold_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rf_rf_threshold_transformer =
        rf_rf_threshold_transformer_builder::lingual< string >( &rf_rf_threshold_transformer::set_name, "name" ) +
        rf_rf_threshold_transformer_builder::lingual< string >( &rf_rf_threshold_transformer::set_threshold_file, "threshold_file" ) +
        rf_rf_threshold_transformer_builder::lingual< string >( &rf_rf_threshold_transformer::set_background_file, "background_file" ) +
        rf_rf_threshold_transformer_builder::numerical< real_t >( &rf_rf_threshold_transformer::set_threshold, "threshold" ) +
        rf_rf_threshold_transformer_builder::numerical< real_t >( &rf_rf_threshold_transformer::set_minimum_frequency, "minimum_frequency" ) +
        rf_rf_threshold_transformer_builder::numerical< real_t >( &rf_rf_threshold_transformer::set_maximum_frequency, "maximum_frequency" ) +
        rf_rf_threshold_transformer_builder::boolean< bool_t >( &rf_rf_threshold_transformer::set_plot, "plot" ) +
        rf_rf_threshold_transformer_builder::lingual< string >( &rf_rf_threshold_transformer::set_plot_key, "plot_key" ) +
        rf_rf_threshold_transformer_builder::lingual< string >( &rf_rf_threshold_transformer::set_plot_name, "plot_name" ) +
        rf_rf_threshold_transformer_builder::lingual< string >( &rf_rf_threshold_transformer::set_chart_title, "chart_title" ) +
        rf_rf_threshold_transformer_builder::lingual< string >( &rf_rf_threshold_transformer::set_axis_title, "axis_title" ) +
        root_builder::object< rf_rf_threshold_transformer >( &root::add, "rf_rf_threshold_transformer" );

}
