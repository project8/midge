#ifndef _midge_types_hh_
#define _midge_types_hh_

#include <stdint.h>

#include <iostream>
using std::cout;
using std::endl;

namespace midge
{
    typedef bool bool_t;
    typedef uint64_t count_t;
    typedef uint8_t state_t;
    typedef int64_t pos_t;
    typedef double real_t;
    typedef double complex_t[2];
}

#endif
