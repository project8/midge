#include "root.hh"
#include "rt_data.hh"
#include "ct_data.hh"
#include "cf_data.hh"
#include "rf_data.hh"
#include "rt_gaussian_producer.hh"
#include "rt_ct_transformer.hh"
#include "ct_cf_fourier_transformer.hh"
#include "cf_rf_transformer.hh"
#include "rt_rf_power_transformer.hh"
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

    rf_data* t_rft = new rf_data();
    t_rft->set_name( "rft" );
    t_root->add( t_rft );

    rf_data* t_rfs = new rf_data();
    t_rfs->set_name( "rfs" );
    t_root->add( t_rfs );

    rt_gaussian_producer* t_rt_in = new rt_gaussian_producer();
    t_rt_in->set_name( "rt_in" );
    t_rt_in->set_power_dbm( -10. );
    t_rt_in->set_impedance_ohm( 50. );
    t_rt_in->set_start_sec( 2000.e-9 );
    t_rt_in->set_stop_sec( 18000.e-9 );
    t_rt_in->set_interval( 1.e-9 );
    t_rt_in->set_size( 1000 );
    t_rt_in->set_stride( 10 );
    t_root->add( t_rt_in );

    rt_ct_transformer* t_rt_ct = new rt_ct_transformer();
    t_rt_ct->set_name( "rt_ct" );
    t_rt_ct->set_mode( "real" );
    t_root->add( t_rt_ct );

    ct_cf_fourier_transformer* t_ct_cf = new ct_cf_fourier_transformer();
    t_ct_cf->set_name( "ct_cf" );
    t_root->add( t_ct_cf );

    cf_rf_transformer* t_cf_rf = new cf_rf_transformer();
    t_cf_rf->set_name( "cf_rf" );
    t_cf_rf->set_mode( "modulus" );
    t_root->add( t_cf_rf );

    rt_rf_power_transformer* t_rt_rf = new rt_rf_power_transformer();
    t_rt_rf->set_name( "rt_rf" );
    t_rt_rf->set_impedance_ohm( 50. );
    t_root->add( t_rt_rf );

    rt_ascii_consumer* t_rt_out = new rt_ascii_consumer();
    t_rt_out->set_name( "rt_out" );
    t_rt_out->set_file( "test_gaussian_producer.signal.txt" );
    t_root->add( t_rt_out );

    rf_ascii_consumer* t_rft_out = new rf_ascii_consumer();
    t_rft_out->set_name( "rft_out" );
    t_rft_out->set_file( "test_gaussian_producer.transform.txt" );
    t_root->add( t_rft_out );

    rf_ascii_consumer* t_rfs_out = new rf_ascii_consumer();
    t_rfs_out->set_name( "rfs_out" );
    t_rfs_out->set_file( "test_gaussian_producer.spectrum.txt" );
    t_root->add( t_rfs_out );

    count_controller* t_c = new count_controller();
    t_c->set_name( "c" );
    t_c->set_count( 1901 );
    t_root->add( t_c );

    t_root->join( "rt_in.out_0:rt.in" );
    t_root->join( "rt.out:rt_ct.in_0" );
    t_root->join( "rt_ct.out_0:ct.in" );
    t_root->join( "ct.out:ct_cf.in_0" );
    t_root->join( "ct_cf.out_0:cf.in" );
    t_root->join( "cf.out:cf_rf.in_0" );
    t_root->join( "cf_rf.out_0:rft.in" );
    t_root->join( "rft.out:rft_out.in_0" );
    t_root->join( "rt.out:rt_rf.in_0" );
    t_root->join( "rt_rf.out_0:rfs.in" );
    t_root->join( "rfs.out:rfs_out.in_0" );
    t_root->join( "rt.out:rt_out.in_0" );
    t_root->join( "c.out_0:rt_in" );

    t_root->run( "c" );

    delete t_root;

    return 0;
}
