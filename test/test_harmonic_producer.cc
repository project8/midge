#include "root.hh"
#include "rt_harmonic_producer.hh"
#include "rt_ascii_consumer.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

#include <cmath>

int main()
{
    root* t_root = new root();

    rt_harmonic_producer* t_rt_in = new rt_harmonic_producer();
    t_rt_in->set_name( "rt_in" );
    t_rt_in->set_power_dbm( -10. );
    t_rt_in->set_impedance_ohm( 50. );
    t_rt_in->set_frequency_hz( 100.e6 );
    t_rt_in->set_phase_deg( 60. );
    t_rt_in->set_start_sec( 2000.e-9 );
    t_rt_in->set_stop_sec( 18000.e-9 );
    t_rt_in->set_interval_sec( 1.e-9 );
    t_rt_in->set_length( 10 );
    t_rt_in->set_size( 1000 );
    t_rt_in->set_stride( 10 );
    t_root->add( t_rt_in );

    rt_ascii_consumer* t_rt_out = new rt_ascii_consumer();
    t_rt_out->set_name( "rt_out" );
    t_rt_out->set_file( "test_harmonic_producer.signal.txt" );
    t_root->add( t_rt_out );

    t_root->join( "rt_in.out_0:rt_out.in_0" );

    t_root->run( "rt_in:rt_out" );

    delete t_root;

    return 0;
}
