#ifndef _midge_rt_rf_power_transformer_hh_
#define _midge_rt_rf_power_transformer_hh_

#include "transformer.hh"
#include "rt_data.hh"
#include "rf_data.hh"
#include "typelist.hh"

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
            void set_impedance_ohm( const real_t& p_impedance );
            const real_t& get_impedance_ohm() const;

        private:
            real_t f_impedance_ohm;

            count_t f_size;
            count_t f_last;
            bool_t f_nyquist;
            real_t* f_in;
            real_t* f_out;
            fftw_complex* f_c_in;
            fftw_complex* f_c_out;
            fftw_plan f_plan;
            real_t f_norm;

        protected:
            bool start_transformer();
            bool execute_transformer();
            bool stop_transformer();
    };

}

#endif
