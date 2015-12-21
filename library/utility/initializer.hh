#ifndef _midge_initializer_hh_
#define _midge_initializer_hh_

#include <cstddef>

#include <iostream>
using std::cout;
using std::endl;

namespace midge
{

    template< class x_type >
    class initializer
    {
        public:
            initializer();
            ~initializer();

        private:
            static x_type* f_instance;
            static int f_count;

        public:
            static char f_data[ sizeof(x_type) ];
    };

    template< class x_type >
    char initializer< x_type >::f_data[ sizeof(x_type) ] =
    { };

    template< class x_type >
    int initializer< x_type >::f_count = 0;

    template< class x_type >
    x_type* initializer< x_type >::f_instance = NULL;

    template< class x_type >
    initializer< x_type >::initializer()
    {
        if( 0 == f_count++ )
        {
            f_instance = new ( (x_type*) (&f_data) ) x_type();
        }
    }
    template< class x_type >
    initializer< x_type >::~initializer()
    {
        if( 0 == --f_count )
        {
            f_instance->~x_type();
        }
    }

}

#endif
