#ifndef _midge_rt_monarch_consumer_hh_
#define _midge_rt_monarch_consumer_hh_

#include "consumer.hh"
#include "rt_data.hh"
#include "typelist.hh"

#include "Monarch.hpp"

namespace midge
{

    class rt_monarch_consumer :
        public _consumer< rt_monarch_consumer, typelist_1( rt_data )>
    {
        public:
            rt_monarch_consumer();
            virtual ~rt_monarch_consumer();

        public:
            accessible( string_t, file )
            accessible( count_t, bits )
            accessible( count_t, record_length )
            accessible( real_t, voltage_minimum )
            accessible( real_t, voltage_maximum )

        public:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
