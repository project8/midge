#include "complex_real_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_complex_real_transformer =
        complex_real_transformer_builder::lingual< string >( &complex_real_transformer::set_name, "name" ) +
        complex_real_transformer_builder::lingual< string >( &complex_real_transformer::set_mode, "mode" ) +
        root_builder::object< complex_real_transformer >( &root::add, "complex_real_transformer" );

}
