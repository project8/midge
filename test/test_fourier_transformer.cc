#include "root.hh"
#include "rt_data.hh"
#include "rf_data.hh"
#include "ct_data.hh"
#include "cf_data.hh"
#include "rt_harmonic_producer.hh"
#include "rt_chirp_producer.hh"
#include "rt_ct_transformer.hh"
#include "ct_cf_fourier_transformer.hh"
#include "cf_rf_transformer.hh"
#include "rt_ascii_consumer.hh"
#include "rf_ascii_consumer.hh"
#include "count_controller.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

#include <cmath>

int main()
{
    root* t_root = new root();

    rt_data* t_rt = new rt_data();
    t_rt->set_name( "rt" );
    t_root->add( t_rt );

    ct_data* t_ct = new ct_data();
    t_ct->set_name( "ct" );
    t_root->add( t_ct );

    cf_data* t_cf = new cf_data();
    t_cf->set_name( "cf" );
    t_root->add( t_cf );

    rf_data* t_rfm = new rf_data();
    t_rfm->set_name( "rfm" );
    t_root->add( t_rfm );

    rf_data* t_rfa = new rf_data();
    t_rfa->set_name( "rfa" );
    t_root->add( t_rfa );

//    rt_harmonic_producer* t_rt_in = new rt_harmonic_producer();
//    t_rt_in->set_name( "rt_in" );
//    t_rt_in->set_power_dbm( -10. );
//    t_rt_in->set_impedance_ohm( 50. );
//    t_rt_in->set_frequency_hz( 100.e6 );
//    t_rt_in->set_phase_deg( 60. );
//    t_rt_in->set_start_sec( 2000.e-9 );
//    t_rt_in->set_stop_sec( 18000.e-9 );
//    t_rt_in->set_interval( 1.e-9 );
//    t_rt_in->set_size( 20000 );
//    t_rt_in->set_stride( 20000 );
//    t_root->add( t_rt_in );

    rt_chirp_producer* t_rt_in = new rt_chirp_producer();
    t_rt_in->set_name( "rt_in" );
    t_rt_in->set_power_dbm( -10. );
    t_rt_in->set_impedance_ohm( 50. );
    t_rt_in->set_start_frequency_hz( 100.e6 );
    t_rt_in->set_stop_frequency_hz( 400.e6 );
    t_rt_in->set_phase_deg( 60. );
    t_rt_in->set_start_sec( 2000.e-9 );
    t_rt_in->set_stop_sec( 18000.e-9 );
    t_rt_in->set_interval( 1.e-9 );
    t_rt_in->set_size( 20000 );
    t_rt_in->set_stride( 20000 );
    t_root->add( t_rt_in );

    rt_ct_transformer* t_rt_ct = new rt_ct_transformer();
    t_rt_ct->set_name( "rt_ct" );
    t_rt_ct->set_mode( "real" );
    t_root->add( t_rt_ct );

    ct_cf_fourier_transformer* t_ct_cf = new ct_cf_fourier_transformer();
    t_ct_cf->set_name( "ct_cf" );
    t_root->add( t_ct_cf );

    cf_rf_transformer* t_cf_rfm = new cf_rf_transformer();
    t_cf_rfm->set_name( "cf_rfm" );
    t_cf_rfm->set_mode( "modulus" );
    t_root->add( t_cf_rfm );

    cf_rf_transformer* t_cf_rfa = new cf_rf_transformer();
    t_cf_rfa->set_name( "cf_rfa" );
    t_cf_rfa->set_mode( "argument" );
    t_root->add( t_cf_rfa );

    rt_ascii_consumer* t_rt_out = new rt_ascii_consumer();
    t_rt_out->set_name( "rt_out" );
    t_rt_out->set_file( "test_fourier_transformer.signal.txt" );
    t_root->add( t_rt_out );

    rf_ascii_consumer* t_rfm_out = new rf_ascii_consumer();
    t_rfm_out->set_name( "rfm_out" );
    t_rfm_out->set_file( "test_fourier_transformer.modulus.txt" );
    t_root->add( t_rfm_out );

    rf_ascii_consumer* t_rfa_out = new rf_ascii_consumer();
    t_rfa_out->set_name( "rfa_out" );
    t_rfa_out->set_file( "test_fourier_transformer.argument.txt" );
    t_root->add( t_rfa_out );

    count_controller* t_c = new count_controller();
    t_c->set_name( "c" );
    t_c->set_count( 1 );
    t_root->add( t_c );

    t_root->join( "rt_in.out_0:rt.in" );
    t_root->join( "rt.out:rt_out.in_0" );
    t_root->join( "rt.out:rt_ct.in_0" );
    t_root->join( "rt_ct.out_0:ct.in" );
    t_root->join( "ct.out:ct_cf.in_0" );
    t_root->join( "ct_cf.out_0:cf.in" );
    t_root->join( "cf.out:cf_rfm.in_0" );
    t_root->join( "cf.out:cf_rfa.in_0" );
    t_root->join( "cf_rfm.out_0:rfm.in" );
    t_root->join( "cf_rfa.out_0:rfa.in" );
    t_root->join( "rfm.out:rfm_out.in_0" );
    t_root->join( "rfa.out:rfa_out.in_0" );
    t_root->join( "c.out_0:rt_in" );

    t_root->run( "c" );

    delete t_root;

    return 0;
}
