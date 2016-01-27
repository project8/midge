#include "midge_builder.hh"

namespace midge
{
    static const int s_midge =
        midge_builder::lingual< std::string >( &diptera::join, "join" ) +
        midge_builder::lingual< std::string >( &diptera::run, "run" );
}
