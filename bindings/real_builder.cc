#include "real_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_real =
        real_builder::lingual< string >( &real::set_name, "name" ) +
        real_builder::numerical< uint64_t >( &real::set_size, "size" ) +
        root_builder::object< real >( &root::add, "real" );

}
