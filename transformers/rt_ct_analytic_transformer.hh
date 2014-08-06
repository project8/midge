#ifndef _midge_rt_ct_analytic_transformer_hh_
#define _midge_rt_ct_analytic_transformer_hh_

#include "transformer.hh"
#include "rt_data.hh"
#include "ct_data.hh"
#include "typelist.hh"

#include "fftw3.h"

namespace midge
{

    class rt_ct_analytic_transformer :
        public _transformer< rt_ct_analytic_transformer, typelist_1( rt_data ), typelist_1( ct_data ) >
    {
        public:
            rt_ct_analytic_transformer();
            virtual ~rt_ct_analytic_transformer();

        private:
            count_t f_size;
            count_t f_under;
            count_t f_nyquist;
            count_t f_over;
            const real_t* f_in;
            complex_t* f_out;
            fftw_complex* f_signal;
            fftw_complex* f_transform;
            fftw_complex* f_analytic;
            fftw_plan f_forward;
            fftw_plan f_backward;
            real_t f_norm;

        protected:
            bool start_transformer();
            bool execute_transformer();
            bool stop_transformer();
    };

}

#endif
