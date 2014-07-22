#ifndef _midge_ct_cf_fourier_transformer_hh_
#define _midge_ct_cf_fourier_transformer_hh_

#include "transformer.hh"
#include "ct_data.hh"
#include "cf_data.hh"
#include "typelist.hh"

#include "fftw3.h"

namespace midge
{

    class ct_cf_fourier_transformer :
        public _transformer< ct_cf_fourier_transformer, typelist_1( ct_data ), typelist_1( cf_data ) >
    {
        public:
            ct_cf_fourier_transformer();
            virtual ~ct_cf_fourier_transformer();

        protected:
            void start_transformer();
            void execute_transformer();
            void stop_transformer();

        private:
            count_t f_size;
            complex_t* f_in;
            complex_t* f_out;
            fftw_plan f_plan;
            real_t f_norm;
    };

}

#endif
