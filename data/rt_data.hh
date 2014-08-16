#ifndef _midge_rt_data_hh_
#define _midge_rt_data_hh_

#include "types.hh"

namespace midge
{

    class rt_data
    {
        public:
            rt_data();
            virtual ~rt_data();

        public:
            real_t* raw();
            const real_t* raw() const;

            void set_size( const count_t& p_size );
            const count_t& get_size() const;

            void set_interval( const real_t& p_interval );
            const real_t& get_interval() const;

            void set_time( const real_t& p_time );
            const real_t& get_time() const;

        protected:
            real_t* f_raw;
            count_t f_size;
            real_t f_interval;
            real_t f_time;
    };

}

#endif
