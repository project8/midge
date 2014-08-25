#include "ct_ctf_fourier_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ct_ctf_fourier_transformer =
        ct_ctf_fourier_transformer_builder::lingual< string >( &ct_ctf_fourier_transformer::set_name, "name" ) +
        ct_ctf_fourier_transformer_builder::numerical< count_t >( &ct_ctf_fourier_transformer::set_length, "length" ) +
        midge_builder::object< ct_ctf_fourier_transformer >( &midge::add, "ct_ctf_fourier_transformer" );

}
