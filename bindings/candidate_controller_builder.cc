#include "candidate_controller_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_candidate_controller =
        candidate_controller_builder::lingual< string >( &candidate_controller::set_name, "name" ) +
        candidate_controller_builder::lingual< string >( &candidate_controller::set_candidate_file, "candidate_file" ) +
        candidate_controller_builder::lingual< string >( &candidate_controller::set_background_file, "background_file" ) +
        candidate_controller_builder::lingual< string >( &candidate_controller::set_background_name, "background_name" ) +
        candidate_controller_builder::numerical< count_t >( &candidate_controller::set_rough_stride, "rough_stride" ) +
        candidate_controller_builder::numerical< count_t >( &candidate_controller::set_fine_stride, "fine_stride" ) +
        candidate_controller_builder::numerical< real_t >( &candidate_controller::set_threshold_ratio, "threshold_ratio" ) +
        candidate_controller_builder::numerical< real_t >( &candidate_controller::set_minimum_time, "minimum_time" ) +
        candidate_controller_builder::numerical< real_t >( &candidate_controller::set_maximum_time, "maximum_time" ) +
        candidate_controller_builder::numerical< real_t >( &candidate_controller::set_minimum_frequency, "minimum_frequency" ) +
        candidate_controller_builder::numerical< real_t >( &candidate_controller::set_maximum_frequency, "maximum_frequency" ) +
        candidate_controller_builder::numerical< real_t >( &candidate_controller::set_minimum_slope, "minimum_slope" ) +
        candidate_controller_builder::numerical< real_t >( &candidate_controller::set_maximum_slope, "maximum_slope" ) +
        candidate_controller_builder::boolean< bool_t >( &candidate_controller::set_plot, "plot" ) +
        candidate_controller_builder::lingual< string >( &candidate_controller::set_plot_title, "plot_title" ) +
        candidate_controller_builder::lingual< string >( &candidate_controller::set_axis_title, "axis_title" ) +
        root_builder::object< candidate_controller >( &root::add, "candidate_controller" );

}
