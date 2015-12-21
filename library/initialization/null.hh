#ifndef _midge_null_hh_
#define _midge_null_hh_

#include "../library/initialization/value.hh"

namespace midge
{

    class null :
        public value
    {
        public:
            null();
            virtual ~null();
            null* clone() const;
    };
}

#endif
