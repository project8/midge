#ifndef _midge_complex_hh_
#define _midge_complex_hh_

#include "operand.hh"
#include "types.hh"

namespace midge
{

    class complex :
        public operand< complex, complex_t >
    {
        public:
            complex();
            virtual ~complex();
    };

}

#endif
