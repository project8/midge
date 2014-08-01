#ifndef _midge_rt_gaussian_producer_hh_
#define _midge_rt_gaussian_producer_hh_

#include "producer.hh"
#include "rt_data.hh"
#include "typelist.hh"

#include "gsl/gsl_rng.h"

namespace midge
{

    class rt_gaussian_producer :
        public _producer< rt_gaussian_producer, typelist_1( rt_data ) >
    {
        public:
            rt_gaussian_producer();
            virtual ~rt_gaussian_producer();

        public:
            void set_power_dbm( const real_t& p_power );
            const real_t& get_power_dbm() const;

            void set_impedance_ohm( const real_t& p_impedance );
            const real_t& get_impedance_ohm() const;

            void set_start_sec( const real_t& p_start );
            const real_t& get_start_sec() const;

            void set_stop_sec( const real_t& p_stop );
            const real_t& get_stop_sec() const;

            void set_seed( const count_t& p_seed );
            const count_t& get_seed() const;

            void set_stride( const count_t& p_stride );
            const count_t& get_stride() const;

            void set_size( const count_t& p_size );
            const count_t& get_size() const;

            void set_interval( const real_t& p_interval );
            const real_t& get_interval() const;

        private:
            real_t f_power_dbm;
            real_t f_impedance_ohm;
            real_t f_start_sec;
            real_t f_stop_sec;
            count_t f_seed;
            count_t f_stride;
            count_t f_size;
            real_t f_interval;

            real_t f_amplitude;
            count_t f_start;
            count_t f_stop;
            gsl_rng* f_rng;

            real_t* f_out;
            count_t f_index;

        protected:
            void initialize_producer();
            bool start_producer();
            bool execute_producer();
            bool stop_producer();
            void finalize_producer();
    };
}

#endif
