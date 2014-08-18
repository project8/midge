#ifndef _midge_rt_root_consumer_hh_
#define _midge_rt_root_consumer_hh_

#include "consumer.hh"
#include "rt_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class rt_root_consumer :
        public _consumer< rt_root_consumer, typelist_1( rt_data )>
    {
        public:
            rt_root_consumer();
            virtual ~rt_root_consumer();

        public:
            accessible( string, file )

        public:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
