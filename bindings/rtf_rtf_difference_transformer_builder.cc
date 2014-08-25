#include "rtf_rtf_difference_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rtf_rtf_difference_transformer =
        rtf_rtf_difference_transformer_builder::lingual< string >( &rtf_rtf_difference_transformer::set_name, "name" ) +
        rtf_rtf_difference_transformer_builder::numerical< count_t >( &rtf_rtf_difference_transformer::set_length, "length" ) +
        midge_builder::object< rtf_rtf_difference_transformer >( &midge::add, "rtf_rtf_difference_transformer" );

}
