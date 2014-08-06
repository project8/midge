#ifndef _midge_rt_rf_power_transformer_hh_
#define _midge_rt_rf_power_transformer_hh_

#include "transformer.hh"
#include "rt_data.hh"
#include "rf_data.hh"
#include "window.hh"
#include "typelist.hh"
#include "macros.hh"

#include "fftw3.h"

namespace midge
{

    class rt_rf_power_transformer :
        public _transformer< rt_rf_power_transformer, typelist_1( rt_data ), typelist_1( rf_data ) >
    {
        public:
            rt_rf_power_transformer();
            virtual ~rt_rf_power_transformer();

        public:
            accessible( real_t, impedance_ohm );
            composable( window, window );

        private:
            count_t f_in_size;
            count_t f_out_size;
            count_t f_last;
            count_t f_nyquist;

            const real_t* f_in;
            real_t f_in_interval;
            real_t f_in_time;

            real_t* f_out;
            real_t f_out_interval;
            real_t f_out_time;

            const real_t* f_multiplier;

            fftw_complex* f_signal;
            fftw_complex* f_transform;
            fftw_plan f_plan;
            real_t f_norm;

        protected:
            bool start_transformer();
            bool execute_transformer();
            bool stop_transformer();
    };

}

#endif
