#include "rt_rf_wigner_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_rf_wigner_transformer =
        rt_rf_wigner_transformer_builder::lingual< string >( &rt_rf_wigner_transformer::set_name, "name" ) +
        rt_rf_wigner_transformer_builder::numerical< count_t >( &rt_rf_wigner_transformer::set_offset, "offset" ) +
        rt_rf_wigner_transformer_builder::numerical< count_t >( &rt_rf_wigner_transformer::set_length, "length" ) +
        root_builder::object< rt_rf_wigner_transformer >( &root::add, "rt_rf_wigner_transformer" );

}
