#include "real_ascii_consumer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_real_ascii_consumer =
        real_ascii_consumer_builder::lingual< string >( &real_ascii_consumer::set_name, "name" ) +
        real_ascii_consumer_builder::lingual< string >( &real_ascii_consumer::set_file, "file" ) +
        root_builder::object< real_ascii_consumer >( &root::add, "real_ascii_consumer" );

}
