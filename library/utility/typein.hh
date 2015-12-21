#ifndef _midge_typein_hh_
#define _midge_typein_hh_

#include "typenode.hh"
#include "typenull.hh"

namespace midge
{

    template< class x_node, class x_query >
    class typein;

    template< class x_head, class x_tail, class x_query >
    class typein< typenode< x_head, x_tail >, x_query >
    {
        public:
            enum
            {
                result = typein< x_tail, x_query >::result
            };
    };

    template< class x_head, class x_tail >
    class typein< typenode< x_head, x_tail >, x_head >
    {
        public:
            enum
            {
                result = true
            };
    };

    template< class x_head, class x_query >
    class typein< _, x_query >
    {
        public:
            enum
            {
                result = false
            };
    };

}

#endif
