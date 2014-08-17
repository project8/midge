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

            void set_time_interval( const real_t& p_time_interval );
            const real_t& get_time_interval() const;

            void set_time_index( const count_t& p_time_index );
            const count_t& get_time_index() const;

        protected:
            real_t* f_raw;
            count_t f_size;
            real_t f_time_interval;
            count_t f_time_index;
    };

}

#endif
