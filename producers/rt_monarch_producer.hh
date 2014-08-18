#ifndef _midge_rt_monarch_producer_hh_
#define _midge_rt_monarch_producer_hh_

#include "producer.hh"
#include "rt_data.hh"
#include "typelist.hh"
#include "macros.hh"

#include "Monarch.hpp"

namespace midge
{

    class rt_monarch_producer :
        public _producer< rt_monarch_producer, typelist_1( rt_data )>
    {
        public:
            rt_monarch_producer();
            virtual ~rt_monarch_producer();

        public:
            accessible( string, file )
            accessible( real_t, begin_sec )
            accessible( real_t, end_sec )
            accessible( count_t, size )
            accessible( count_t, stride )
            accessible( count_t, length )

        public:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
