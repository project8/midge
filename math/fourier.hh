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
            friend class singleton< fourier > ;

        private:
            fourier();
            virtual ~fourier();

        public:
            template< class x_type >
            x_type* allocate( const count_t& p_size )
            {
                x_type* t_pointer;
                f_mutex.lock();
                t_pointer = (x_type*) (fftw_malloc( sizeof(x_type) * p_size ));
                f_mutex.unlock();
                return t_pointer;
            }

            template< class x_type >
            void free( x_type* p_pointer )
            {
                f_mutex.lock();
                fftw_free( p_pointer );
                f_mutex.unlock();
                return;
            }

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
