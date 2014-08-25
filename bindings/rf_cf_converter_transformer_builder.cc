 #include "rf_cf_converter_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rf_cf_converter_transformer =
        rf_cf_converter_transformer_builder::lingual< string >( &rf_cf_converter_transformer::set_name, "name" ) +
        rf_cf_converter_transformer_builder::lingual< string >( &rf_cf_converter_transformer::set_mode, "mode" ) +
        rf_cf_converter_transformer_builder::numerical< count_t >( &rf_cf_converter_transformer::set_length, "length" ) +
        midge_builder::object< rf_cf_converter_transformer >( &midge::add, "rf_cf_converter_transformer" );

}
