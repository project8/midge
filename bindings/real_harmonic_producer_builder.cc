#include "real_harmonic_producer_builder.hh"
#include "root_builder.hh"

namespace midge
{

    static const int s_real_harmonic_producer =
        real_harmonic_producer_builder::lingual< string >( &real_harmonic_producer::set_name, "name" ) +
        real_harmonic_producer_builder::numerical< double >( &real_harmonic_producer::set_amplitude, "amplitude" ) +
        real_harmonic_producer_builder::numerical< double >( &real_harmonic_producer::set_frequency, "frequency" ) +
        real_harmonic_producer_builder::numerical< double >( &real_harmonic_producer::set_phase, "phase" ) +
        root_builder::object< real_harmonic_producer >( &root::add, "real_harmonic_producer" );

}
