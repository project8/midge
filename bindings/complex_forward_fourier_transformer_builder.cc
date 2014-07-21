#include "complex_forward_fourier_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_complex_forward_fourier_transformer =
        complex_forward_fourier_transformer_builder::lingual< string >( &complex_forward_fourier_transformer::set_name, "name" ) +
        root_builder::object< complex_forward_fourier_transformer >( &root::add, "complex_forward_fourier_transformer" );

}
