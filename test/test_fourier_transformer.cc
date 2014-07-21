#include "root.hh"
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
    root* t_root = new root();


    real* t_real_in = new real();
    t_real_in->set_name( "real_in" );
    t_real_in->set_size( 8192 );
    t_root->add( t_real_in );

    complex* t_complex_in = new complex();
    t_complex_in->set_name( "complex_in" );
    t_complex_in->set_size( 8192 );
    t_root->add( t_complex_in );

    complex* t_complex_out = new complex();
    t_complex_out->set_name( "complex_out" );
    t_complex_out->set_size( 8192 );
    t_root->add( t_complex_out );

    real* t_real_out = new real();
    t_real_out->set_name( "real_out" );
    t_real_out->set_size( 8192 );
    t_root->add( t_real_out );

    real* t_imaginary_out = new real();
    t_imaginary_out->set_name( "imaginary_out" );
    t_imaginary_out->set_size( 8192 );
    t_root->add( t_imaginary_out );

    real* t_modulus_out = new real();
    t_modulus_out->set_name( "modulus_out" );
    t_modulus_out->set_size( 8192 );
    t_root->add( t_modulus_out );

    real* t_argument_out = new real();
    t_argument_out->set_name( "argument_out" );
    t_argument_out->set_size( 8192 );
    t_root->add( t_argument_out );


    real_harmonic_producer* t_producer = new real_harmonic_producer();
    t_producer->set_name( "producer" );
    t_producer->set_amplitude( 1.5 );
    t_producer->set_frequency( 1. / 64. );
    t_producer->set_phase( M_PI / 4. );
    t_root->add( t_producer );

    real_complex_transformer* t_real_complex_real = new real_complex_transformer();;
    t_real_complex_real->set_name( "real_complex_real" );
    t_real_complex_real->set_mode( "real" );
    t_root->add( t_real_complex_real );

    complex_forward_fourier_transformer* t_fft = new complex_forward_fourier_transformer();
    t_fft->set_name( "fft" );
    t_root->add( t_fft );

    complex_real_transformer* t_complex_real_real = new complex_real_transformer();
    t_complex_real_real->set_name( "complex_real_real" );
    t_complex_real_real->set_mode( "real" );
    t_root->add( t_complex_real_real );

    complex_real_transformer* t_complex_real_imaginary = new complex_real_transformer();
    t_complex_real_imaginary->set_name( "complex_real_imaginary" );
    t_complex_real_imaginary->set_mode( "imaginary" );
    t_root->add( t_complex_real_imaginary );

    complex_real_transformer* t_complex_real_modulus = new complex_real_transformer();
    t_complex_real_modulus->set_name( "complex_real_modulus" );
    t_complex_real_modulus->set_mode( "modulus" );
    t_root->add( t_complex_real_modulus );

    complex_real_transformer* t_complex_real_argument = new complex_real_transformer();
    t_complex_real_argument->set_name( "complex_real_argument" );
    t_complex_real_argument->set_mode( "argument" );
    t_root->add( t_complex_real_argument );

    real_ascii_consumer* t_consumer_real = new real_ascii_consumer();
    t_consumer_real->set_name( "consumer_real" );
    t_consumer_real->set_file( "fourier_transform.real.txt" );
    t_root->add( t_consumer_real );

    real_ascii_consumer* t_consumer_imaginary = new real_ascii_consumer();
    t_consumer_imaginary->set_name( "consumer_imaginary" );
    t_consumer_imaginary->set_file( "fourier_transform.imaginary.txt" );
    t_root->add( t_consumer_imaginary );

    real_ascii_consumer* t_consumer_modulus = new real_ascii_consumer();
    t_consumer_modulus->set_name( "consumer_modulus" );
    t_consumer_modulus->set_file( "fourier_transform.modulus.txt" );
    t_root->add( t_consumer_modulus );

    real_ascii_consumer* t_consumer_argument = new real_ascii_consumer();
    t_consumer_argument->set_name( "consumer_argument" );
    t_consumer_argument->set_file( "fourier_transform.argument.txt" );
    t_root->add( t_consumer_argument );


    t_root->join( "producer.out_0:real_in.in" );
    t_root->join( "real_in.out:real_complex_real.in_0" );
    t_root->join( "real_complex_real.out_0:complex_in.in" );
    t_root->join( "complex_in.out:fft.in_0" );
    t_root->join( "fft.out_0:complex_out.in" );
    t_root->join( "complex_out.out:complex_real_real.in_0" );
    t_root->join( "complex_out.out:complex_real_imaginary.in_0" );
    t_root->join( "complex_out.out:complex_real_modulus.in_0" );
    t_root->join( "complex_out.out:complex_real_argument.in_0" );
    t_root->join( "complex_real_real.out_0:real_out.in" );
    t_root->join( "complex_real_imaginary.out_0:imaginary_out.in" );
    t_root->join( "complex_real_modulus.out_0:modulus_out.in" );
    t_root->join( "complex_real_argument.out_0:argument_out.in" );
    t_root->join( "real_out.out:consumer_real.in_0" );
    t_root->join( "imaginary_out.out:consumer_imaginary.in_0" );
    t_root->join( "modulus_out.out:consumer_modulus.in_0" );
    t_root->join( "argument_out.out:consumer_argument.in_0" );


    t_root->run( "producer" );


    delete t_root;


    return 0;
}
