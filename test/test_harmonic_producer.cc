#include "root.hh"
#include "rt_data.hh"
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

    rt_data* t_data = new rt_data();
    t_data->set_name( "data" );
    t_root->add( t_data );

    rt_harmonic_producer* t_producer = new rt_harmonic_producer();
    t_producer->set_name( "producer" );
    t_producer->set_power_dbm( -10. );
    t_producer->set_impedance_ohm( 50. );
    t_producer->set_frequency_hz( 10.e6 );
    t_producer->set_phase_deg( 60. );
    t_producer->set_start_sec( 500.e-9 );
    t_producer->set_stop_sec( 1500.e-9 );
    t_producer->set_interval( 1.e-9 );
    t_producer->set_size( 2000 );
    t_producer->set_stride( 2000 );
    t_root->add( t_producer );

    rt_ascii_consumer* t_consumer = new rt_ascii_consumer();
    t_consumer->set_name( "consumer" );
    t_consumer->set_file( "test_harmonic_producer.txt" );
    t_root->add( t_consumer );


    t_root->join( "producer.out_0:data.in" );
    t_root->join( "data.out:consumer.in_0" );

    t_root->run( "producer" );

    delete t_root;

    return 0;
}
