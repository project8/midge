#include "rt_binary_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rt_binary_consumer =
        rt_binary_consumer_builder::lingual< string >( &rt_binary_consumer::set_name, "name" ) +
        rt_binary_consumer_builder::lingual< string >( &rt_binary_consumer::set_file, "file" ) +
        midge_builder::object< rt_binary_consumer >( &midge::add, "rt_binary_consumer" );

}
