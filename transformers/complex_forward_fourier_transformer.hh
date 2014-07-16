#ifndef _midge_complex_forward_fourier_transformer_hh_
#define _midge_complex_forward_fourier_transformer_hh_

#include "transformer.hh"
#include "complex.hh"
#include "typelist.hh"

#include "fftw3.h"

namespace midge
{

    class complex_forward_fourier_transformer :
        public transformer< complex_forward_fourier_transformer, typelist_1( complex ), typelist_1( complex ) >
    {
        public:
            complex_forward_fourier_transformer();
            virtual ~complex_forward_fourier_transformer();

        protected:
            void initialize_transformer();
            void deinitialize_transformer();
            void update_transformer();

        private:
            fftw_plan f_plan;
    };

}

#endif
