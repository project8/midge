#include "real.hh"
#include "complex.hh"
#include "real_harmonic_producer.hh"
#include "real_complex_transformer.hh"
#include "complex_forward_fourier_transformer.hh"
#include "complex_real_transformer.hh"
#include "real_ascii_consumer.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

#include <cmath>

int main()
{
    real t_real_in;
    t_real_in.set_name( "real_in" );
    t_real_in.allocate( 8192 );

    complex t_complex_in;
    t_complex_in.set_name( "complex_in" );
    t_complex_in.allocate( 8192 );

    complex t_complex_out;
    t_complex_out.set_name( "complex_out" );
    t_complex_out.allocate( 8192 );

    real t_real_out;
    t_real_out.set_name( "real_out" );
    t_real_out.allocate( 8192 );

    real_harmonic_producer t_producer;
    t_producer.set_name( "test_producer" );
    t_producer.set_amplitude( 1.5 );
    t_producer.set_frequency( 1. / 64. );
    t_producer.set_phase( M_PI / 4. );

    real_complex_transformer t_real_complex;
    t_real_complex.set_name( "test_real_complex" );
    t_real_complex.set_mode( real_complex_transformer::e_real );

    complex_forward_fourier_transformer t_fft;
    t_fft.set_name( "test_fft" );

    complex_real_transformer t_complex_real;
    t_complex_real.set_name( "test_complex_real_modulus" );
    t_complex_real.set_mode( complex_real_transformer::e_modulus );

    real_ascii_consumer t_consumer;
    t_consumer.set_name( "test_consumer" );
    t_consumer.set_filename( "test_setup_two.txt" );

    t_producer.output( "set_output_0" )->connect( &t_real_in );
    t_real_in.input( "set_input" )->connect( &t_producer );
    t_real_in.output( "add_output" )->connect( &t_real_complex );
    t_real_complex.input( "set_input_0" )->connect( &t_real_in );
    t_real_complex.output( "set_output_0" )->connect( &t_complex_in );
    t_complex_in.input( "set_input" )->connect( &t_real_complex );
    t_complex_in.output( "add_output" )->connect( &t_fft );
    t_fft.input( "set_input_0" )->connect( &t_complex_in );
    t_fft.output( "set_output_0" )->connect( &t_complex_out );
    t_complex_out.input( "set_input" )->connect( &t_fft );
    t_complex_out.output( "add_output" )->connect( &t_complex_real );
    t_complex_real.input( "set_input_0" )->connect( &t_complex_out );
    t_complex_real.output( "set_output_0" )->connect( &t_real_out );
    t_real_out.input( "set_input" )->connect( &t_complex_real );
    t_real_out.output( "add_output")->connect( &t_consumer );
    t_consumer.input( "set_input_0" )->connect( &t_real_out );

    t_producer.initialize();
    t_producer.execute();
    t_producer.finalize();

    return 0;
}
