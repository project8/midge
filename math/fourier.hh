#ifndef midge_fourier_hh_
#define midge_fourier_hh_

#include "singleton.hh"
#include "mutex.hh"
#include "types.hh"

#include "fftw3.h"

namespace midge
{

    class fourier :
        public singleton< fourier >
    {
        public:
            friend class singleton< fourier >;

        private:
            fourier();
            virtual ~fourier();

        public:
            fftw_complex* allocate( const count_t& p_size );
            void free( fftw_complex* );

            fftw_plan forward( const count_t& p_size, fftw_complex* p_input, fftw_complex* p_output );
            fftw_plan backward( const count_t& p_size, fftw_complex* p_input, fftw_complex* p_output );
            void destroy( fftw_plan p_plan );

            void execute( fftw_plan p_plan );
            void execute( fftw_plan p_plan, fftw_complex* p_input, fftw_complex* p_output );

        private:
            mutex f_mutex;
    };

}

#endif
