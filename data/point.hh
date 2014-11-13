#ifndef _midge_point_hh_
#define _midge_point_hh_

#include "types.hh"
#include "macros.hh"

namespace midge
{

    class point
    {
        public:
            ;accessible_static( real_t, threshold )
            ;accessible_static( real_t, high_factor )
            ;accessible_static( real_t, high_power )
            ;accessible_static( real_t, low_factor )
            ;accessible_static( real_t, low_power )

        public:
            point();
            point( const point& a_copy );
            ~point();

            void update();

        public:
            ;referrable( count_t, id )
            ;referrable( real_t, time )
            ;referrable( real_t, frequency )
            ;referrable( real_t, ratio )
            ;referrable( real_t, score )
    };

}

#endif
