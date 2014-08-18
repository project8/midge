#ifndef midge_rt_sinusoidal_producer_hh_
#define midge_rt_sinusoidal_producer_hh_

#include "producer.hh"
#include "rt_data.hh"
#include "typelist.hh"

namespace midge
{

    class rt_sinusoidal_producer :
        public _producer< rt_sinusoidal_producer, typelist_1( rt_data ) >
    {
        public:
            rt_sinusoidal_producer();
            virtual ~rt_sinusoidal_producer();

        public:
            accessible( real_t, impedance_ohm )
            accessible( real_t, carrier_power_dbm )
            accessible( real_t, carrier_frequency_hz )
            accessible( real_t, carrier_phase_deg )
            accessible( real_t, signal_amplitude_hz )
            accessible( real_t, signal_frequency_hz )
            accessible( real_t, signal_phase_deg )
            accessible( real_t, begin_sec )
            accessible( real_t, end_sec )
            accessible( real_t, start_sec )
            accessible( real_t, stop_sec )
            accessible( real_t, interval_sec )
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
