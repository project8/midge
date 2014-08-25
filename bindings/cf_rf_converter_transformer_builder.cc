#include "cf_rf_converter_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_cf_rf_converter_transformer =
        cf_rf_converter_transformer_builder::lingual< string >( &cf_rf_converter_transformer::set_name, "name" ) +
        cf_rf_converter_transformer_builder::lingual< string >( &cf_rf_converter_transformer::set_mode, "mode" ) +
        cf_rf_converter_transformer_builder::numerical< count_t >( &cf_rf_converter_transformer::set_length, "length" ) +
        midge_builder::object< cf_rf_converter_transformer >( &midge::add, "cf_rf_converter_transformer" );

}
