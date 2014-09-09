#ifndef _midge_line_data_hh_
#define _midge_line_data_hh_

#include "line.hh"

#include <vector>
using std::vector;

namespace midge
{

    class line_data
    {
        public:
            line_data();
            virtual ~line_data();

        public:
            ;referrable( vector< pointer< line > >, lines )
            ;accessible( count_t, size )
            ;accessible( real_t, time_interval )
            ;accessible( count_t, time_index )
            ;accessible( real_t, frequency_interval )
            ;accessible( count_t, frequency_index )
    };

}

#endif
