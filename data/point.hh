#ifndef _midge_point_hh_
#define _midge_point_hh_

#include "types.hh"
#include "macros.hh"

namespace midge
{

    class point
    {
        public:
            point();
            ~point();

        public:
            ;referrable( count_t, id )
            ;referrable( real_t, time )
            ;referrable( real_t, frequency )
            ;referrable( real_t, ratio )
    };

}

#endif
