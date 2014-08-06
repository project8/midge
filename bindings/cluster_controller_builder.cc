#include "cluster_controller_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_cluster_controller =
        cluster_controller_builder::lingual< string >( &cluster_controller::set_name, "name" ) +
        cluster_controller_builder::lingual< string >( &cluster_controller::set_candidate_file, "candidate_file" ) +
        cluster_controller_builder::lingual< string >( &cluster_controller::set_background_file, "background_file" ) +
        cluster_controller_builder::lingual< string >( &cluster_controller::set_background_name, "background_name" ) +
        cluster_controller_builder::numerical< count_t >( &cluster_controller::set_stride, "stride" ) +
        cluster_controller_builder::numerical< real_t >( &cluster_controller::set_threshold, "threshold" ) +
        cluster_controller_builder::numerical< real_t >( &cluster_controller::set_minimum_time, "minimum_time" ) +
        cluster_controller_builder::numerical< real_t >( &cluster_controller::set_maximum_time, "maximum_time" ) +
        cluster_controller_builder::numerical< real_t >( &cluster_controller::set_minimum_frequency, "minimum_frequency" ) +
        cluster_controller_builder::numerical< real_t >( &cluster_controller::set_maximum_frequency, "maximum_frequency" ) +
        cluster_controller_builder::boolean< bool_t >( &cluster_controller::set_plot, "plot" ) +
        cluster_controller_builder::lingual< string >( &cluster_controller::set_plot_title, "plot_title" ) +
        cluster_controller_builder::lingual< string >( &cluster_controller::set_axis_title, "axis_title" ) +
        root_builder::object< cluster_controller >( &root::add, "cluster_controller" );

}
