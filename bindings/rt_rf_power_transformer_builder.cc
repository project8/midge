#include "rt_rf_power_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_rf_power_transformer =
        rt_rf_power_transformer_builder::lingual< string >( &rt_rf_power_transformer::set_name, "name" ) +
        rt_rf_power_transformer_builder::numerical< real_t >( &rt_rf_power_transformer::set_impedance_ohm, "impedance_ohm" ) +
        root_builder::object< rt_rf_power_transformer >( &root::add, "rt_rf_power_transformer" );

}
