#ifndef _midge_rt_rf_wigner_transformer_hh_
#define _midge_rt_rf_wigner_transformer_hh_

#include "transformer.hh"
#include "rt_data.hh"
#include "rf_data.hh"
#include "typelist.hh"

#include "fftw3.h"

namespace midge
{

    class rt_rf_wigner_transformer :
        public _transformer< rt_rf_wigner_transformer, typelist_1( rt_data ), typelist_1( rf_data ) >
    {
        public:
            rt_rf_wigner_transformer();
            virtual ~rt_rf_wigner_transformer();

        public:
            void set_offset( const count_t& p_offset );
            const count_t& get_offset() const;

            void set_length( const count_t& p_length );
            const count_t& get_length() const;

        private:
            count_t f_offset;
            count_t f_length;

            count_t f_in_size;
            count_t f_out_size;
            count_t f_under;
            count_t f_nyquist;
            count_t f_over;
            real_t f_norm;
            real_t* f_in;
            real_t* f_out;
            fftw_complex* f_signal;
            fftw_complex* f_transform;
            fftw_complex* f_analytic;
            fftw_complex* f_correlation;
            fftw_complex* f_distribution;
            fftw_plan f_forward;
            fftw_plan f_backward;
            fftw_plan f_final;

        protected:
            bool start_transformer();
            bool execute_transformer();
            bool stop_transformer();
    };

}

#endif
