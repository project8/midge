#include "rt_root_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rt_root_consumer =
        rt_root_consumer_builder::lingual< string >( &rt_root_consumer::set_name, "name" ) +
        rt_root_consumer_builder::lingual< string >( &rt_root_consumer::set_file, "file" ) +
        midge_builder::object< rt_root_consumer >( &midge::add, "rt_root_consumer" );

}
