#include "cf_ct_fourier_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_cf_ct_fourier_transformer =
        cf_ct_fourier_transformer_builder::lingual< string >( &cf_ct_fourier_transformer::set_name, "name" ) +
        root_builder::object< cf_ct_fourier_transformer >( &root::add, "cf_ct_fourier_transformer" );

}
