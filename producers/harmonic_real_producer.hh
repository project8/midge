#ifndef _midge_harmonic_real_producer_hh_
#define _midge_harmonic_real_producer_hh_

#include "producer.hh"
#include "real.hh"
#include "typelist.hh"

namespace midge
{

    class harmonic_real_producer :
        public producer< harmonic_real_producer, typelist_1( real ) >
    {
        public:
            harmonic_real_producer();
            virtual ~harmonic_real_producer();

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

        protected:
            void execute_producer();
    };
}

#endif
