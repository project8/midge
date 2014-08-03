#include "ct_data_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_ct_data =
        ct_data_builder::lingual< string >( &ct_data::set_name, "name" ) +
        ct_data_builder::numerical< count_t >( &ct_data::set_size, "size" ) +
        ct_data_builder::numerical< real_t >( &ct_data::set_interval, "interval" ) +
        ct_data_builder::numerical< real_t >( &ct_data::set_time, "time" ) +
        root_builder::object< ct_data >( &root::add, "ct_data" );

}
