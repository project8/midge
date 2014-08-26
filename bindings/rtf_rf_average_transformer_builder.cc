#include "rtf_rf_average_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rtf_rf_average_transformer =
        rtf_rf_average_transformer_builder::lingual< string >( &rtf_rf_average_transformer::set_name, "name" ) +
        rtf_rf_average_transformer_builder::numerical< count_t >( &rtf_rf_average_transformer::set_length, "length" ) +
        midge_builder::object< rtf_rf_average_transformer >( &midge::add, "rtf_rf_average_transformer" );

}
