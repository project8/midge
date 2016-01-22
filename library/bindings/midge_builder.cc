#include "midge_builder.hh"

namespace midge
{
    static const int s_midge =
        midge_builder::lingual< std::string >( &midge::join, "join" ) +
        midge_builder::lingual< std::string >( &midge::run, "run" );
}
