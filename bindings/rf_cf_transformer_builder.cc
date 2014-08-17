#include "rf_cf_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rf_cf_transformer =
        rf_cf_transformer_builder::lingual< string >( &rf_cf_transformer::set_name, "name" ) +
        rf_cf_transformer_builder::lingual< string >( &rf_cf_transformer::set_mode, "mode" ) +
        rf_cf_transformer_builder::numerical< count_t >( &rf_cf_transformer::set_length, "length" ) +
        root_builder::object< rf_cf_transformer >( &root::add, "rf_cf_transformer" );

}
