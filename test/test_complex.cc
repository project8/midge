#include "ct_data.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

int main()
{

    ct_data t_data;
    t_data.set_name( "test_data" );
    t_data.set_size( 4194304 );

    t_data.start();
    t_data.execute();
    t_data.stop();

    return 0;

}
