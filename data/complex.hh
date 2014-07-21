#ifndef _midge_complex_hh_
#define _midge_complex_hh_

#include "data.hh"
#include "types.hh"

namespace midge
{

    class complex :
        public _data< complex, complex_t >
    {
        public:
            complex();
            virtual ~complex();
    };

}

#endif
