#ifndef _midge_cf_data_hh_
#define _midge_cf_data_hh_

#include "data.hh"
#include "types.hh"

namespace midge
{

    class cf_data :
        public _data< cf_data, complex_t >
    {
        public:
            cf_data();
            virtual ~cf_data();
    };

}

#endif
