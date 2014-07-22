#include "root.hh"
#include "rt_data.hh"
#include "rf_data.hh"
#include "ct_data.hh"
#include "cf_data.hh"
#include "rt_harmonic_producer.hh"
#include "rt_ct_transformer.hh"
#include "ct_cf_fourier_transformer.hh"
#include "cf_rf_transformer.hh"
#include "rf_ascii_consumer.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

#include <cmath>

int main()
{
    root* t_root = new root();


    rt_data* t_rt_in = new rt_data();
    t_rt_in->set_name( "rt_in" );
    t_root->add( t_rt_in );

    ct_data* t_complex_in = new ct_data();
    t_complex_in->set_name( "ct_in" );
    t_root->add( t_complex_in );

    cf_data* t_complex_out = new cf_data();
    t_complex_out->set_name( "cf_out" );
    t_root->add( t_complex_out );

    rf_data* t_rf_real_out = new rf_data();
    t_rf_real_out->set_name( "rf_real_out" );
    t_root->add( t_rf_real_out );

    rf_data* t_rf_imaginary_out = new rf_data();
    t_rf_imaginary_out->set_name( "rf_imaginary_out" );
    t_root->add( t_rf_imaginary_out );

    rf_data* t_rf_modulus_out = new rf_data();
    t_rf_modulus_out->set_name( "rf_modulus_out" );
    t_root->add( t_rf_modulus_out );

    rf_data* t_rf_argument_out = new rf_data();
    t_rf_argument_out->set_name( "rf_argument_out" );
    t_root->add( t_rf_argument_out );

    rt_harmonic_producer* t_rt = new rt_harmonic_producer();
    t_rt->set_name( "rt" );
    t_rt->set_power_dbm( -10. );
    t_rt->set_impedance_ohm( 50. );
    t_rt->set_frequency_hz( 100.e6 );
    t_rt->set_phase_deg( 60. );
    t_rt->set_interval( 1.e-9 );
    t_rt->set_size( 8192 );
    t_rt->set_stride( 8192 );
    t_root->add( t_rt );

    rt_ct_transformer* t_rt_ct_real = new rt_ct_transformer();;
    t_rt_ct_real->set_name( "rt_ct_real" );
    t_rt_ct_real->set_mode( "real" );
    t_root->add( t_rt_ct_real );

    ct_cf_fourier_transformer* t_ct_cf_fourier = new ct_cf_fourier_transformer();
    t_ct_cf_fourier->set_name( "ct_cf_fourier" );
    t_root->add( t_ct_cf_fourier );

    cf_rf_transformer* t_cf_rf_real = new cf_rf_transformer();
    t_cf_rf_real->set_name( "cf_rf_real" );
    t_cf_rf_real->set_mode( "real" );
    t_root->add( t_cf_rf_real );

    cf_rf_transformer* t_cf_rf_imaginary = new cf_rf_transformer();
    t_cf_rf_imaginary->set_name( "cf_rf_imaginary" );
    t_cf_rf_imaginary->set_mode( "imaginary" );
    t_root->add( t_cf_rf_imaginary );

    cf_rf_transformer* t_cf_rf_modulus = new cf_rf_transformer();
    t_cf_rf_modulus->set_name( "cf_rf_modulus" );
    t_cf_rf_modulus->set_mode( "modulus" );
    t_root->add( t_cf_rf_modulus );

    cf_rf_transformer* t_cf_rf_argument = new cf_rf_transformer();
    t_cf_rf_argument->set_name( "cf_rf_argument" );
    t_cf_rf_argument->set_mode( "argument" );
    t_root->add( t_cf_rf_argument );

    rf_ascii_consumer* t_rf_real = new rf_ascii_consumer();
    t_rf_real->set_name( "rf_real" );
    t_rf_real->set_file( "test_fourier_transform.real.txt" );
    t_root->add( t_rf_real );

    rf_ascii_consumer* t_rf_imaginary = new rf_ascii_consumer();
    t_rf_imaginary->set_name( "rf_imaginary" );
    t_rf_imaginary->set_file( "test_fourier_transform.imaginary.txt" );
    t_root->add( t_rf_imaginary );

    rf_ascii_consumer* t_rf_modulus = new rf_ascii_consumer();
    t_rf_modulus->set_name( "rf_modulus" );
    t_rf_modulus->set_file( "test_fourier_transform.modulus.txt" );
    t_root->add( t_rf_modulus );

    rf_ascii_consumer* t_rf_argument = new rf_ascii_consumer();
    t_rf_argument->set_name( "rf_argument" );
    t_rf_argument->set_file( "test_fourier_transform.argument.txt" );
    t_root->add( t_rf_argument );


    t_root->join( "rt.out_0:rt_in.in" );
    t_root->join( "rt_in.out:rt_ct_real.in_0" );
    t_root->join( "rt_ct_real.out_0:ct_in.in" );
    t_root->join( "ct_in.out:ct_cf_fourier.in_0" );
    t_root->join( "ct_cf_fourier.out_0:cf_out.in" );
    t_root->join( "cf_out.out:cf_rf_real.in_0" );
    t_root->join( "cf_out.out:cf_rf_imaginary.in_0" );
    t_root->join( "cf_out.out:cf_rf_modulus.in_0" );
    t_root->join( "cf_out.out:cf_rf_argument.in_0" );
    t_root->join( "cf_rf_real.out_0:rf_real_out.in" );
    t_root->join( "cf_rf_imaginary.out_0:rf_imaginary_out.in" );
    t_root->join( "cf_rf_modulus.out_0:rf_modulus_out.in" );
    t_root->join( "cf_rf_argument.out_0:rf_argument_out.in" );
    t_root->join( "rf_real_out.out:rf_real.in_0" );
    t_root->join( "rf_imaginary_out.out:rf_imaginary.in_0" );
    t_root->join( "rf_modulus_out.out:rf_modulus.in_0" );
    t_root->join( "rf_argument_out.out:rf_argument.in_0" );


    t_root->run( "rt" );


    delete t_root;


    return 0;
}
