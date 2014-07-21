#include "root.hh"
#include "real.hh"
#include "real_harmonic_producer.hh"
#include "real_ascii_consumer.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

#include <cmath>

int main()
{
    root* t_root = new root();

    real* t_real = new real();
    t_real->set_name( "test_real" );
    t_real->set_size( 1024 );
    t_root->add( t_real );

    real_harmonic_producer* t_producer = new real_harmonic_producer();
    t_producer->set_name( "test_producer" );
    t_producer->set_amplitude( 1.5 );
    t_producer->set_frequency( 1. / 256. );
    t_producer->set_phase( M_PI / 4. );
    t_root->add( t_producer );

    real_ascii_consumer* t_consumer = new real_ascii_consumer();
    t_consumer->set_name( "test_consumer" );
    t_consumer->set_file( "test_harmonic_producer.txt" );
    t_root->add( t_consumer );


    t_root->join( "test_producer.out_0:test_real.in" );
    t_root->join( "test_real.out:test_consumer.in_0" );

    t_root->run( "test_producer" );

    delete t_root;

    return 0;
}
