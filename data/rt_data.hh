#ifndef _midge_rt_data_hh_
#define _midge_rt_data_hh_

#include "data.hh"
#include "types.hh"

namespace midge
{

    class rt_data :
        public _data< rt_data, real_t >
    {
        public:
            rt_data();
            virtual ~rt_data();
    };

}

#endif
