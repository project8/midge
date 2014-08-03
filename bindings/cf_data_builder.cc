#include "cf_data_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_cf_data =
        cf_data_builder::lingual< string >( &cf_data::set_name, "name" ) +
        cf_data_builder::numerical< count_t >( &cf_data::set_size, "size" ) +
        cf_data_builder::numerical< real_t >( &cf_data::set_interval, "interval" ) +
        cf_data_builder::numerical< real_t >( &cf_data::set_time, "time" ) +
        root_builder::object< cf_data >( &root::add, "cf_data" );

}
