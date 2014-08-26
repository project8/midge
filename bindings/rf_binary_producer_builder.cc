#include "rf_binary_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rf_binary_producer =
        rf_binary_producer_builder::lingual< string >( &rf_binary_producer::set_name, "name" ) +
        rf_binary_producer_builder::lingual< string >( &rf_binary_producer::set_file, "file" ) +
        rf_binary_producer_builder::numerical< count_t >( &rf_binary_producer::set_length, "length" ) +
        midge_builder::object< rf_binary_producer >( &midge::add, "rf_binary_producer" );

}
