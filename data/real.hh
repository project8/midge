#ifndef _midge_real_hh_
#define _midge_real_hh_

#include "data.hh"
#include "types.hh"

namespace midge
{

    class real :
        public _data< real, real_t >
    {
        public:
            real();
            virtual ~real();
    };

}

#endif
