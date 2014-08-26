#include "rf_ascii_producer_builder.hh"
#include "midge_builder.hh"

namespace midge
{

    static const int s_rf_ascii_producer =
        rf_ascii_producer_builder::lingual< string >( &rf_ascii_producer::set_name, "name" ) +
        rf_ascii_producer_builder::lingual< string >( &rf_ascii_producer::set_file, "file" ) +
        rf_ascii_producer_builder::numerical< count_t >( &rf_ascii_producer::set_length, "length" ) +
        midge_builder::object< rf_ascii_producer >( &midge::add, "rf_ascii_producer" );

}
