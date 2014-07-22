#ifndef _midge_count_controller_hh_
#define _midge_count_controller_hh_

#include "controller.hh"
#include "typelist.hh"

namespace midge
{

    class count_controller :
        public _controller< count_controller, typelist_0(),typelist_10( node, node, node, node, node, node, node, node, node, node ) >
    {
        public:
            count_controller();
            virtual ~count_controller();
    };

}

#endif
