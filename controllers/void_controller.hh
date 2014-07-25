#ifndef _midge_void_controller_hh_
#define _midge_void_controller_hh_

#include "controller.hh"
#include "typelist.hh"

namespace midge
{

    class void_controller :
        public _controller< void_controller, typelist_0(),typelist_10( node, node, node, node, node, node, node, node, node, node ) >
    {
        public:
            void_controller();
            virtual ~void_controller();
    };

}

#endif
