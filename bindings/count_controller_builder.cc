#include "count_controller_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_count_controller =
        count_controller_builder::lingual< string >( &count_controller::set_name, "name" ) +
        count_controller_builder::numerical< count_t >( &count_controller::set_count, "count" ) +
        root_builder::object< count_controller >( &root::add, "count_controller" );

}
