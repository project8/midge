#include "complex.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

int main()
{

    complex t_complex;
    t_complex.set_name( "test_complex" );
    t_complex.set_size( 4194304 );

    t_complex.initialize();
    t_complex.execute();
    t_complex.finalize();

    return 0;

}
