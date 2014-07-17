#include "real.hh"
#include "real_harmonic_producer.hh"
#include "real_ascii_consumer.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

#include <cmath>

int main()
{
    real t_real;
    t_real.set_name( "test_real" );
    t_real.allocate( 128 );

    real_harmonic_producer t_producer;
    t_producer.set_name( "test_producer" );
    t_producer.set_amplitude( 1.5 );
    t_producer.set_frequency( 1. / 256. );
    t_producer.set_phase( M_PI / 4. );

    real_ascii_consumer t_consumer;
    t_consumer.set_name( "test_consumer" );
    t_consumer.set_filename( "test_setup_one.txt" );

    t_real.input( "set_input" )->connect( &t_producer );
    t_producer.output( "set_output_0" )->connect( &t_real );

    t_real.output( "add_output" )->connect( &t_consumer );
    t_consumer.input( "set_input_0" )->connect( &t_real );

    t_producer.initialize();
    t_producer.execute();
    t_producer.execute();
    t_producer.execute();
    t_producer.execute();
    t_producer.finalize();

    return 0;
}
