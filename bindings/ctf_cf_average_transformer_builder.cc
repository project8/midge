#include "ctf_cf_average_transformer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_ctf_cf_average_transformer =
        ctf_cf_average_transformer_builder::lingual< string >( &ctf_cf_average_transformer::set_name, "name" ) +
        ctf_cf_average_transformer_builder::numerical< count_t >( &ctf_cf_average_transformer::set_length, "length" ) +
        midge_builder::object< ctf_cf_average_transformer >( &midge::add, "ctf_cf_average_transformer" );

}
