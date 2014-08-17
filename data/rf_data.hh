#ifndef _midge_rf_data_hh_
#define _midge_rf_data_hh_

#include "types.hh"

namespace midge
{

    class rf_data
    {
        public:
            rf_data();
            virtual ~rf_data();

        public:
            real_t* raw();
            const real_t* raw() const;

            void set_size( const count_t& p_size );
            const count_t& get_size() const;

            void set_time_interval( const real_t& p_time_interval );
            const real_t& get_time_interval() const;

            void set_time_index( const count_t& p_time_index );
            const count_t& get_time_index() const;

            void set_frequency_interval( const real_t& p_frequency_interval );
            const real_t& get_frequency_interval() const;

            void set_frequency_index( const count_t& p_frequency_index );
            const count_t& get_frequency_index() const;

        protected:
            real_t* f_raw;
            count_t f_size;
            real_t f_time_interval;
            count_t f_time_index;
            real_t f_frequency_interval;
            count_t f_frequency_index;
    };

}

#endif
