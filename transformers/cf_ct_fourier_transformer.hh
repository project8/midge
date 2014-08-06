#ifndef _midge_cf_ct_fourier_transformer_hh_
#define _midge_cf_ct_fourier_transformer_hh_

#include "transformer.hh"
#include "cf_data.hh"
#include "ct_data.hh"
#include "typelist.hh"

#include "fftw3.h"

namespace midge
{

    class cf_ct_fourier_transformer :
        public _transformer< cf_ct_fourier_transformer, typelist_1( cf_data ), typelist_1( ct_data ) >
    {
        public:
            cf_ct_fourier_transformer();
            virtual ~cf_ct_fourier_transformer();

        private:
            count_t f_size;

            const complex_t* f_in;
            real_t f_in_interval;
            real_t f_in_time;

            complex_t* f_out;
            real_t f_out_interval;
            real_t f_out_time;

            fftw_plan f_plan;
            real_t f_norm;

        protected:
            bool start_transformer();
            bool execute_transformer();
            bool stop_transformer();
    };

}

#endif
