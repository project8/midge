#include "rf_rf_sum_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rf_rf_sum_transformer =
        rf_rf_sum_transformer_builder::lingual< string >( &rf_rf_sum_transformer::set_name, "name" ) +
        rf_rf_sum_transformer_builder::numerical< count_t >( &rf_rf_sum_transformer::set_length, "length" ) +
        midge_builder::object< rf_rf_sum_transformer >( &midge::add, "rf_rf_sum_transformer" );

}
