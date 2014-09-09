#ifndef _midge_line_hh_
#define _midge_line_hh_

#include "point.hh"
#include "pointer.hh"

#include <list>
using std::list;

namespace midge
{

    class line
    {
        public:
            line();
            ~line();

        public:
            ;referrable( count_t, id )
            ;referrable( real_t, time )
            ;referrable( real_t, frequency )
            ;referrable( real_t, ratio )
            ;referrable( real_t, window )
            ;referrable( real_t, slope )
            ;referrable( real_t, duration )
            ;referrable( real_t, correlation )
            ;referrable( real_t, deviation )

            ;referrable( count_t, local_count )
            ;referrable( real_t, local_sum )

            ;referrable( real_t, local_r_sum )
            ;referrable( real_t, local_rt_sum )
            ;referrable( real_t, local_rf_sum )
            ;referrable( real_t, local_rtt_sum )
            ;referrable( real_t, local_rff_sum )
            ;referrable( real_t, local_rtf_sum )

            ;referrable( list< list< pointer< point > > >, local )

            ;referrable( count_t, global_count )
            ;referrable( real_t, global_sum )

            ;referrable( real_t, global_r_sum )
            ;referrable( real_t, global_rt_sum )
            ;referrable( real_t, global_rf_sum )
            ;referrable( real_t, global_rtt_sum )
            ;referrable( real_t, global_rff_sum )
            ;referrable( real_t, global_rtf_sum )

            ;referrable( list< pointer< point > >, global )
    };

}
#endif
