#include "ct_cf_fourier_transformer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_ct_cf_fourier_transformer =
        ct_cf_fourier_transformer_builder::lingual< string >( &ct_cf_fourier_transformer::set_name, "name" ) +
        ct_cf_fourier_transformer_builder::numerical< count_t >( &ct_cf_fourier_transformer::set_length, "length" ) +
        root_builder::object< ct_cf_fourier_transformer >( &root::add, "ct_cf_fourier_transformer" );

}
