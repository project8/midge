#ifndef _midge_real_harmonic_producer_hh_
#define _midge_real_harmonic_producer_hh_

#include "producer.hh"
#include "real.hh"
#include "typelist.hh"

namespace midge
{

    class real_harmonic_producer :
        public _producer< real_harmonic_producer, typelist_1( real ) >
    {
        public:
            real_harmonic_producer();
            virtual ~real_harmonic_producer();

        public:
            void set_amplitude( const real_t& p_amplitude );
            const real_t& get_amplitude() const;

            void set_frequency( const real_t& p_frequency );
            const real_t& get_frequency() const;

            void set_phase( const real_t& p_phase );
            const real_t& get_phase() const;

        private:
            real_t f_amplitude;
            real_t f_frequency;
            real_t f_phase;
            real_t f_internal;
            real_t* f_raw;
            uint64_t f_size;

        protected:
            void initialize_producer();
            void execute_producer();
    };
}

#endif
