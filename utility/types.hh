#ifndef _midge_types_hh_
#define _midge_types_hh_

#include <stdint.h>

#include <string>
using std::string;

namespace midge
{
    typedef bool bool_t;
    typedef uint16_t state_t;
    typedef uint64_t count_t;
    typedef int64_t pos_t;
    typedef double real_t;
    typedef double complex_t[2];
    typedef string string_t;
}

#endif
