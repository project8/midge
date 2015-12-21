#ifndef _midge_typeif_hh_
#define _midge_typeif_hh_

#include "../library/utility/typenode.hh"
#include "../library/utility/typenull.hh"

namespace midge
{

    template< bool x_condition, class x_then, class x_else >
    class typeif
    {
        public:
            typedef x_then result;
    };

    template< class x_then, class x_else >
    class typeif< false, x_then, x_else >
    {
        public:
            typedef x_else result;
    };

}

#endif
