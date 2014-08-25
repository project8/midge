#include "ctf_ctf_difference_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ctf_ctf_difference_transformer =
        ctf_ctf_difference_transformer_builder::lingual< string >( &ctf_ctf_difference_transformer::set_name, "name" ) +
        ctf_ctf_difference_transformer_builder::numerical< count_t >( &ctf_ctf_difference_transformer::set_length, "length" ) +
        midge_builder::object< ctf_ctf_difference_transformer >( &midge::add, "ctf_ctf_difference_transformer" );

}
