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
            complex_t* f_in;
            complex_t* f_out;
            fftw_plan f_plan;
            real_t f_norm;

        protected:
            void start_transformer();
            void execute_transformer();
            void stop_transformer();
    };

}

#endif
