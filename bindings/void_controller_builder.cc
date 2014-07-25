#include "void_controller_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_void_controller =
        void_controller_builder::lingual< string >( &void_controller::set_name, "name" ) +
        root_builder::object< void_controller >( &root::add, "void_controller" );

}
