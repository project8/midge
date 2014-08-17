#include "cf_rf_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_cf_rf_transformer =
        cf_rf_transformer_builder::lingual< string >( &cf_rf_transformer::set_name, "name" ) +
        cf_rf_transformer_builder::lingual< string >( &cf_rf_transformer::set_mode, "mode" ) +
        cf_rf_transformer_builder::numerical< count_t >( &cf_rf_transformer::set_length, "length" ) +
        root_builder::object< cf_rf_transformer >( &root::add, "cf_rf_transformer" );

}
