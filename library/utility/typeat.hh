#ifndef _midge_typeat_hh_
#define _midge_typeat_hh_

#include "../library/utility/typenode.hh"
#include "../library/utility/typenull.hh"

namespace midge
{

    template< class x_node, int x_index >
    class typeat;

    template< class x_head, class x_tail, int x_index >
    class typeat< typenode< x_head, x_tail >, x_index >
    {
        public:
            typedef typename typeat< x_tail, x_index - 1 >::result result;
    };

    template< class x_head, class x_tail >
    class typeat< typenode< x_head, x_tail >, 0 >
    {
        public:
            typedef x_head result;
    };

    template< int x_index >
    class typeat< _, x_index >
    {
        public:
            typedef _ result;
    };

}

#endif
