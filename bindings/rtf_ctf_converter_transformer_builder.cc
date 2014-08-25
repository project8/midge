#include "rtf_ctf_converter_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rtf_ctf_converter_transformer =
        rtf_ctf_converter_transformer_builder::lingual< string >( &rtf_ctf_converter_transformer::set_name, "name" ) +
        rtf_ctf_converter_transformer_builder::lingual< string >( &rtf_ctf_converter_transformer::set_mode, "mode" ) +
        rtf_ctf_converter_transformer_builder::numerical< count_t >( &rtf_ctf_converter_transformer::set_length, "length" ) +
        midge_builder::object< rtf_ctf_converter_transformer >( &midge::add, "rtf_ctf_converter_transformer" );

}
