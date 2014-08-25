#include "rf_binary_consumer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rf_binary_consumer =
        rf_binary_consumer_builder::lingual< string >( &rf_binary_consumer::set_name, "name" ) +
        rf_binary_consumer_builder::lingual< string >( &rf_binary_consumer::set_file, "file" ) +
        midge_builder::object< rf_binary_consumer >( &midge::add, "rf_binary_consumer" );

}
