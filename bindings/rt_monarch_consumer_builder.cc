#include "rt_monarch_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rt_monarch_consumer =
        rt_monarch_consumer_builder::lingual< string >( &rt_monarch_consumer::set_name, "name" ) +
        rt_monarch_consumer_builder::lingual< string >( &rt_monarch_consumer::set_file, "file" ) +
        rt_monarch_consumer_builder::numerical< count_t >( &rt_monarch_consumer::set_bits, "bits" ) +
        rt_monarch_consumer_builder::numerical< count_t >( &rt_monarch_consumer::set_record_length, "record_length" ) +
        rt_monarch_consumer_builder::numerical< real_t >( &rt_monarch_consumer::set_voltage_minimum, "voltage_minimum" ) +
        rt_monarch_consumer_builder::numerical< real_t >( &rt_monarch_consumer::set_voltage_maximum, "voltage_maximum" ) +
        midge_builder::object< rt_monarch_consumer >( &midge::add, "rt_monarch_consumer" );

}
