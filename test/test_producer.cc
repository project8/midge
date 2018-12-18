/*
 * test_producer.cc
 *
 *  Created on: Dec 17, 2018
 *      Author: N.S. Oblath
 */

#include <iostream>

#include "producer.hh"

using std::cout;
using std::endl;

namespace midge
{
    namespace test
    {
        class int_data
        {
            public:
                int_data() : f_value( 5 ) {}
                ~int_data() {}

                void set_data( int value ) {f_value = value;}
                int get_data() const {return f_value;}

            private:
                int f_value;
        };

        class int_producer : public midge::_producer< int_producer, type_list< int_data > >
        {
            public:
                int_producer() : f_length( 10 ) {}
                virtual ~int_producer() {}

            public:
                void set_length( unsigned length ) {f_length = length;}
                unsigned get_length() const {return f_length;}

            private:
                unsigned f_length;

            public:
                virtual void initialize() {}
                virtual void execute( midge::diptera* a_midge = nullptr ) {}
                virtual void finalize() {}

        };

    }
}

using namespace midge;
using namespace midge::test;

int main()
{
    int_producer prod;

    return 0;
}



