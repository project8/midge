#ifndef _midge_rt_harmonic_producer_hh_
#define _midge_rt_harmonic_producer_hh_

#include "producer.hh"
#include "rt_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class rt_harmonic_producer :
        public _producer< rt_harmonic_producer, typelist_1( rt_data ) >
    {
        public:
            rt_harmonic_producer();
            virtual ~rt_harmonic_producer();

        public:
            accessible( real_t, power_dbm )
            accessible( real_t, impedance_ohm )
            accessible( real_t, frequency_hz )
            accessible( real_t, phase_deg )
            accessible( real_t, begin_sec )
            accessible( real_t, end_sec )
            accessible( real_t, start_sec )
            accessible( real_t, stop_sec )
            accessible( real_t, interval_sec )
            accessible( count_t, length )
            accessible( count_t, size )
            accessible( count_t, stride )

        public:
            void initialize();
            void execute();
            void finalize();
    };
}

#endif
