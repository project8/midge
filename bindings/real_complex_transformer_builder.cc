#include "real_complex_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_real_complex_transformer =
        real_complex_transformer_builder::lingual< string >( &real_complex_transformer::set_name, "name" ) +
        real_complex_transformer_builder::lingual< string >( &real_complex_transformer::set_mode, "mode" ) +
        root_builder::object< real_complex_transformer >( &root::add, "real_complex_transformer" );

}
