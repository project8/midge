#include "root.hh"
#include "rt_data.hh"
#include "ct_data.hh"
#include "rt_harmonic_producer.hh"
#include "rt_ct_analytic_transformer.hh"
#include "ct_rt_transformer.hh"
#include "rt_ascii_consumer.hh"
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

    rt_data* t_rtm = new rt_data();
    t_rtm->set_name( "rtr" );
    t_root->add( t_rtm );

    rt_data* t_rta = new rt_data();
    t_rta->set_name( "rti" );
    t_root->add( t_rta );

    rt_harmonic_producer* t_rt_in = new rt_harmonic_producer();
    t_rt_in->set_name( "rt_in" );
    t_rt_in->set_power_dbm( -10. );
    t_rt_in->set_impedance_ohm( 50. );
    t_rt_in->set_frequency_hz( 2.e6 );
    t_rt_in->set_phase_deg( 60. );
    t_rt_in->set_start_sec( 2000.e-9 );
    t_rt_in->set_stop_sec( 18000.e-9 );
    t_rt_in->set_interval( 1.e-9 );
    t_rt_in->set_size( 20000 );
    t_rt_in->set_stride( 20000 );
    t_root->add( t_rt_in );

    rt_ct_analytic_transformer* t_rt_ct = new rt_ct_analytic_transformer();
    t_rt_ct->set_name( "rt_ct" );
    t_root->add( t_rt_ct );

    ct_rt_transformer* t_ct_rtr = new ct_rt_transformer();
    t_ct_rtr->set_name( "ct_rtr" );
    t_ct_rtr->set_mode( "real" );
    t_root->add( t_ct_rtr );

    ct_rt_transformer* t_ct_rti = new ct_rt_transformer();
    t_ct_rti->set_name( "ct_rti" );
    t_ct_rti->set_mode( "imaginary" );
    t_root->add( t_ct_rti );

    rt_ascii_consumer* t_rt_out = new rt_ascii_consumer();
    t_rt_out->set_name( "rt_out" );
    t_rt_out->set_file( "test_analytic_transformer.signal.txt" );
    t_root->add( t_rt_out );

    rt_ascii_consumer* t_rtr_out = new rt_ascii_consumer();
    t_rtr_out->set_name( "rtr_out" );
    t_rtr_out->set_file( "test_analytic_transformer.real.txt" );
    t_root->add( t_rtr_out );

    rt_ascii_consumer* t_rti_out = new rt_ascii_consumer();
    t_rti_out->set_name( "rti_out" );
    t_rti_out->set_file( "test_analytic_transformer.imaginary.txt" );
    t_root->add( t_rti_out );

    count_controller* t_c = new count_controller();
    t_c->set_name( "c" );
    t_c->set_count( 1 );
    t_root->add( t_c );

    t_root->join( "rt_in.out_0:rt.in" );
    t_root->join( "rt.out:rt_out.in_0" );
    t_root->join( "rt.out:rt_ct.in_0" );
    t_root->join( "rt_ct.out_0:ct.in" );
    t_root->join( "ct.out:ct_rtr.in_0" );
    t_root->join( "ct.out:ct_rti.in_0" );
    t_root->join( "ct_rtr.out_0:rtr.in" );
    t_root->join( "ct_rti.out_0:rti.in" );
    t_root->join( "rtr.out:rtr_out.in_0" );
    t_root->join( "rti.out:rti_out.in_0" );
    t_root->join( "c.out_0:rt_in" );

    t_root->run( "c" );

    delete t_root;

    return 0;
}
