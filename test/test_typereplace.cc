/*
 * test_typereplace.cc
 *
 *  Created on: Dec 15, 2018
 *      Author: N.S. Oblath
 */

#include "typereplace.hh"
#include "typelist.hh"

#include <iostream>

using namespace midge;

using std::cout;
using std::endl;

int main()
{

    static_assert(
        std::is_same<
            type_replace< type_list<int, char, int, float, int>, int, float >,
            type_list<float, char, float, float, float>
        >::value, "Oops"
    );

    return 0;

}



