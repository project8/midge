#ifndef _midge_rf_data_hh_
#define _midge_rf_data_hh_

#include "data.hh"
#include "types.hh"

namespace midge
{

    class rf_data :
        public _data< rf_data, real_t >
    {
        public:
            rf_data();
            virtual ~rf_data();

        public:
            void set_center_time( const real_t& p_time );
            const real_t& get_center_time() const;

        protected:
            real_t f_center_time;
    };

}

#endif
