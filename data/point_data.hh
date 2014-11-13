#ifndef _midge_point_data_hh_
#define _midge_point_data_hh_

#include "point.hh"

#include <vector>
using std::vector;

namespace midge
{

    class point_data
    {
        public:
            point_data();
            virtual ~point_data();

        public:
            ;referrable( vector< point >, points )
            ;accessible( count_t, size )
            ;accessible( real_t, time_interval )
            ;accessible( count_t, time_index )
            ;accessible( real_t, frequency_interval )
            ;accessible( count_t, frequency_index )
    };

}

#endif
