#include "root.hh"
#include "rt_data.hh"
#include "rf_data.hh"
#include "ct_data.hh"
#include "cf_data.hh"
#include "rt_harmonic_producer.hh"
#include "rt_ct_transformer.hh"
#include "ct_cf_fourier_transformer.hh"
#include "cf_rf_power_transformer.hh"
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
    t_rf_real_out->set_name( "rf_out" );
    t_root->add( t_rf_real_out );

    rt_harmonic_producer* t_rt = new rt_harmonic_producer();
    t_rt->set_name( "rt_harmonic" );
    t_rt->set_power_dbm( -10. );
    t_rt->set_impedance_ohm( 50. );
    t_rt->set_frequency_hz( 100.e6 );
    t_rt->set_phase_deg( 60. );
    t_rt->set_start_sec( 1000.e-9 );
    t_rt->set_stop_sec( 9000.e-9 );
    t_rt->set_interval( 1.e-9 );
    t_rt->set_size( 10000 );
    t_rt->set_stride( 10000 );
    t_root->add( t_rt );

    rt_ct_transformer* t_rt_ct_real = new rt_ct_transformer();
    ;
    t_rt_ct_real->set_name( "rt_ct_real" );
    t_rt_ct_real->set_mode( "real" );
    t_root->add( t_rt_ct_real );

    ct_cf_fourier_transformer* t_ct_cf_fourier = new ct_cf_fourier_transformer();
    t_ct_cf_fourier->set_name( "ct_cf_fourier" );
    t_root->add( t_ct_cf_fourier );

    cf_rf_power_transformer* t_cf_rf_power = new cf_rf_power_transformer();
    t_cf_rf_power->set_name( "cf_rf_power" );
    t_cf_rf_power->set_impedance_ohm( 50. );
    t_root->add( t_cf_rf_power );

    rf_ascii_consumer* t_rf_real = new rf_ascii_consumer();
    t_rf_real->set_name( "rf_ascii" );
    t_rf_real->set_file( "test_power_transformer.txt" );
    t_root->add( t_rf_real );

    t_root->join( "rt_harmonic.out_0:rt_in.in" );
    t_root->join( "rt_in.out:rt_ct_real.in_0" );
    t_root->join( "rt_ct_real.out_0:ct_in.in" );
    t_root->join( "ct_in.out:ct_cf_fourier.in_0" );
    t_root->join( "ct_cf_fourier.out_0:cf_out.in" );
    t_root->join( "cf_out.out:cf_rf_power.in_0" );
    t_root->join( "cf_rf_power.out_0:rf_out.in");
    t_root->join( "rf_out.out:rf_ascii.in_0" );

    t_root->run( "rt_harmonic" );

    delete t_root;

    return 0;
}
