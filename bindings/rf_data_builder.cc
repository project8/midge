#include "rf_data_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rf_data =
        rf_data_builder::lingual< string >( &rf_data::set_name, "name" ) +
        rf_data_builder::numerical< count_t >( &rf_data::set_size, "size" ) +
        rf_data_builder::numerical< real_t >( &rf_data::set_interval, "interval" ) +
        rf_data_builder::numerical< real_t >( &rf_data::set_center_time, "center_time" ) +
        root_builder::object< rf_data >( &root::add, "rf_data" );

}
