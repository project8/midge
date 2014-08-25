#include "ctf_rtf_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ctf_rtf_transformer =
        ctf_rtf_transformer_builder::lingual< string >( &ctf_rtf_transformer::set_name, "name" ) +
        ctf_rtf_transformer_builder::lingual< string >( &ctf_rtf_transformer::set_mode, "mode" ) +
        ctf_rtf_transformer_builder::numerical< count_t >( &ctf_rtf_transformer::set_length, "length" ) +
        midge_builder::object< ctf_rtf_transformer >( &midge::add, "ctf_rtf_transformer" );

}
