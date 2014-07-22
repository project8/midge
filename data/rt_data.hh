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

        public:
            void set_start_time( const real_t& p_time );
            const real_t& get_start_time() const;

        protected:
            real_t f_start_time;
    };

}

#endif
