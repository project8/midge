#include "ctf_ct_fourier_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ctf_ct_fourier_transformer =
        ctf_ct_fourier_transformer_builder::lingual< string >( &ctf_ct_fourier_transformer::set_name, "name" ) +
        ctf_ct_fourier_transformer_builder::numerical< count_t >( &ctf_ct_fourier_transformer::set_length, "length" ) +
        midge_builder::object< ctf_ct_fourier_transformer >( &midge::add, "ctf_ct_fourier_transformer" );

}
