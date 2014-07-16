#ifndef _midge_real_hh_
#define _midge_real_hh_

#include "operand.hh"
#include "types.hh"

namespace midge
{

    class real :
        public operand< real, real_t >
    {
        public:
            real();
            virtual ~real();
    };

}

#endif
