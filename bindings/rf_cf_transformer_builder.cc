#include "rf_cf_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rf_cf_transformer =
        rf_cf_transformer_builder::lingual< string >( &rf_cf_transformer::set_name, "name" ) +
        rf_cf_transformer_builder::lingual< string >( &rf_cf_transformer::set_mode, "mode" ) +
        root_builder::object< rf_cf_transformer >( &root::add, "rf_cf_transformer" );

}
