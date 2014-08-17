#ifndef midge_fourier_hh_
#define midge_fourier_hh_

#include "singleton.hh"
#include "mutex.hh"
#include "types.hh"

#include "fftw3.h"

namespace midge
{

    typedef struct fftw_plan_s fourier_t;

    class fourier :
        public singleton< fourier >
    {
        public:
            friend class singleton< fourier >;

        private:
            fourier();
            virtual ~fourier();

        public:
            real_t* allocate_real( const count_t& p_size );
            void free_real( real_t* p_pointer );

            complex_t* allocate_complex( const count_t& p_size );
            void free_complex( complex_t* p_pointer );

            fourier_t* forward( const count_t& p_size, complex_t* p_input, complex_t* p_output );
            fourier_t* backward( const count_t& p_size, complex_t* p_input, complex_t* p_output );
            void destroy( fourier_t* p_fourier );

            void execute( fourier_t* p_fourier );
            void execute( fourier_t* p_fourier, complex_t* p_input, complex_t* p_output );

        private:
            mutex f_mutex;
    };

}

#endif
