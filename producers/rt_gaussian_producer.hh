#ifndef _midge_rt_gaussian_producer_hh_
#define _midge_rt_gaussian_producer_hh_

#include "producer.hh"
#include "rt_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class rt_gaussian_producer :
        public _producer< rt_gaussian_producer, typelist_1( rt_data ) >
    {
        public:
            rt_gaussian_producer();
            virtual ~rt_gaussian_producer();

        public:
            accessible( real_t, power_dbm )
            accessible( real_t, impedance_ohm )
            accessible( real_t, begin_sec )
            accessible( real_t, end_sec )
            accessible( real_t, start_sec )
            accessible( real_t, stop_sec )
            accessible( real_t, interval_sec )
            accessible( count_t, seed )
            accessible( count_t, length )
            accessible( count_t, size )
            accessible( count_t, stride )

        protected:
            void initialize();
            void execute();
            void finalize();
    };
}

#endif
