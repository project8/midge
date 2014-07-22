#include "rt_ascii_consumer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rt_ascii_consumer =
        rt_ascii_consumer_builder::lingual< string >( &rt_ascii_consumer::set_name, "name" ) +
        rt_ascii_consumer_builder::lingual< string >( &rt_ascii_consumer::set_file, "file" ) +
        root_builder::object< rt_ascii_consumer >( &root::add, "rt_ascii_consumer" );

}
