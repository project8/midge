#include "fourier.hh"

namespace midge
{

    fourier::fourier() :
            f_mutex()
    {
    }
    fourier::~fourier()
    {
    }

    complex_t* fourier::allocate_complex( const count_t& p_size )
    {
        complex_t* t_pointer;
        f_mutex.lock();
        t_pointer = fftw_alloc_complex( p_size );
        f_mutex.unlock();
        return t_pointer;
    }
    void fourier::free_complex( complex_t* p_pointer )
    {
        f_mutex.lock();
        fftw_free( p_pointer );
        f_mutex.unlock();
        return;
    }

    real_t* fourier::allocate_real( const count_t& p_size )
    {
        real_t* t_pointer;
        f_mutex.lock();
        t_pointer = fftw_alloc_real( p_size );
        f_mutex.unlock();
        return t_pointer;
    }
    void fourier::free_real( real_t* p_pointer )
    {
        f_mutex.lock();
        fftw_free( p_pointer );
        f_mutex.unlock();
        return;
    }

    fourier_t* fourier::forward( const count_t& p_size, complex_t* p_input, complex_t* p_output )
    {
        fourier_t* t_pointer;
        f_mutex.lock();
        t_pointer = fftw_plan_dft_1d( p_size, p_input, p_output, FFTW_FORWARD, FFTW_MEASURE );
        f_mutex.unlock();
        return t_pointer;
    }
    fourier_t* fourier::backward( const count_t& p_size, complex_t* p_input, complex_t* p_output )
    {
        fourier_t* t_pointer;
        f_mutex.lock();
        t_pointer = fftw_plan_dft_1d( p_size, p_input, p_output, FFTW_BACKWARD, FFTW_MEASURE );
        f_mutex.unlock();
        return t_pointer;
    }
    void fourier::destroy( fourier_t* p_fourier )
    {
        f_mutex.lock();
        fftw_destroy_plan( p_fourier );
        f_mutex.unlock();
        return;
    }

    void fourier::execute( fourier_t* p_fourier )
    {
        fftw_execute( p_fourier );
        return;
    }
    void fourier::execute( fourier_t* p_fourier, complex_t* p_input, complex_t* p_output )
    {
        fftw_execute_dft( p_fourier, p_input, p_output );
        return;
    }

}
