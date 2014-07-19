#ifndef _midge_typelength_hh_
#define _midge_typelength_hh_

#include "typenode.hh"
#include "typenull.hh"

namespace midge
{

    template< class x_node >
    class typelength;

    template< class x_head, class x_tail >
    class typelength< typenode< x_head, x_tail > >
    {
        public:
            enum
            {
                result = typelength< x_tail >::result + 1
            };
    };

    template< >
    class typelength< typenull >
    {
        public:
            enum
            {
                result = 0
            };
    };

}

#endif
