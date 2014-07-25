#include "root.hh"
#include "rt_data.hh"
#include "rf_data.hh"
#include "rt_chirp_producer.hh"
#include "rt_rf_wigner_transformer.hh"
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

    rf_data* t_rfm = new rf_data();
    t_rfm->set_name( "rf" );
    t_root->add( t_rfm );

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
    t_rt_in->set_size( 1000 );
    t_rt_in->set_stride( 10 );
    t_root->add( t_rt_in );

    rt_rf_wigner_transformer* t_rt_rf = new rt_rf_wigner_transformer();
    t_rt_rf->set_name( "rt_rf" );
    t_rt_rf->set_offset( 500 );
    t_rt_rf->set_length( 100 );
    t_root->add( t_rt_rf );

    rt_ascii_consumer* t_rt_out = new rt_ascii_consumer();
    t_rt_out->set_name( "rt_out" );
    t_rt_out->set_file( "test_wigner_transformer.signal.txt" );
    t_root->add( t_rt_out );

    rf_ascii_consumer* t_rf_out = new rf_ascii_consumer();
    t_rf_out->set_name( "rf_out" );
    t_rf_out->set_file( "test_wigner_transformer.spectrum.txt" );
    t_root->add( t_rf_out );

    count_controller* t_c = new count_controller();
    t_c->set_name( "c" );
    t_c->set_count( 1901 );
    t_root->add( t_c );

    t_root->join( "rt_in.out_0:rt.in" );
    t_root->join( "rt.out:rt_out.in_0" );
    t_root->join( "rt.out:rt_rf.in_0" );
    t_root->join( "rt_rf.out_0:rf.in" );
    t_root->join( "rf.out:rf_out.in_0" );
    t_root->join( "c.out_0:rt_in" );

    t_root->run( "c" );

    delete t_root;

    return 0;
}
