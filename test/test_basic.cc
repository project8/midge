#include "real.hh"
#include "harmonic_real_producer.hh"
#include "ascii_real_consumer.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

#include <cmath>

int main()
{
    real t_real;
    t_real.set_name( "test_real" );
    t_real.set_size( 128 );

    harmonic_real_producer t_producer;
    t_producer.set_name( "test_producer" );
    t_producer.set_amplitude( 1.5 );
    t_producer.set_frequency( 0.05 );
    t_producer.set_phase( M_PI / 4. );

    ascii_real_consumer t_consumer;
    t_consumer.set_name( "test_consumer" );
    t_consumer.set_filename( "test_basic.txt" );

    t_real.input( "set_input" )->connect( &t_producer );
    t_producer.output( "set_output_0" )->connect( &t_real );

    t_real.output( "add_output" )->connect( &t_consumer );
    t_consumer.input( "set_input_0" )->connect( &t_real );

    t_producer.initialize();
    t_producer.execute();
    t_producer.execute();
    t_producer.finalize();

    return 0;
}
