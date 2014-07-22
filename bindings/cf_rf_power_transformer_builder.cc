#include "cf_rf_power_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_cf_rf_power_transformer =
        cf_rf_power_transformer_builder::lingual< string >( &cf_rf_power_transformer::set_name, "name" ) +
        cf_rf_power_transformer_builder::numerical< real_t >( &cf_rf_power_transformer::set_impedance_ohm, "impedance_ohm" ) +
        root_builder::object< cf_rf_power_transformer >( &root::add, "cf_rf_power_transformer" );

}
