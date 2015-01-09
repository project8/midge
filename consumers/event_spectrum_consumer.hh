#ifndef _midge_event_spectrum_consumer_hh_
#define _midge_event_spectrum_consumer_hh_

#include "consumer.hh"
#include "event_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class event_spectrum_consumer :
        public _consumer< event_spectrum_consumer, typelist_1( event_data )>
    {
        public:
            event_spectrum_consumer();
            virtual ~event_spectrum_consumer();

        public:
            accessible( string, file )
            accessible( real_t, frequency_mhz )
            accessible( real_t, field_gauss )
            accessible( real_t, start_kev )
            accessible( real_t, stop_kev )
            accessible( count_t, count )

        public:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
