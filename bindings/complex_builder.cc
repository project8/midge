#include "complex_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_complex =
        complex_builder::lingual< string >( &complex::set_name, "name" ) +
        complex_builder::numerical< uint64_t >( &complex::set_size, "size" ) +
        root_builder::object< complex >( &root::add, "complex" );

}
