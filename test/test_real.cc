#include "real.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

int main()
{

    real t_real;
    t_real.set_name( "test_real" );
    t_real.set_size( 1024 );

    t_real.initialize();
    t_real.execute();
    t_real.finalize();

    return 0;

}
