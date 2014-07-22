#include "rf_ascii_consumer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_rf_ascii_consumer =
        rf_ascii_consumer_builder::lingual< string >( &rf_ascii_consumer::set_name, "name" ) +
        rf_ascii_consumer_builder::lingual< string >( &rf_ascii_consumer::set_file, "file" ) +
        root_builder::object< rf_ascii_consumer >( &root::add, "rf_ascii_consumer" );

}
