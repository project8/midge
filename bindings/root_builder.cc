#include "root_builder.hh"

namespace midge
{
    static const int s_root =
        root_builder::lingual< string >( &root::join, "join" ) +
        root_builder::lingual< string >( &root::run, "run" );
}
