#ifndef _midge_ct_data_hh_
#define _midge_ct_data_hh_

#include "data.hh"
#include "types.hh"

namespace midge
{

    class ct_data :
        public _data< ct_data, complex_t >
    {
        public:
            ct_data();
            virtual ~ct_data();
    };

}

#endif
