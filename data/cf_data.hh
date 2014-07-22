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

        public:
            void set_center_time( const real_t& p_time );
            const real_t& get_center_time() const;

        protected:
            real_t f_center_time;
    };

}

#endif
