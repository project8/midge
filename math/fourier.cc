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

    fftw_complex* fourier::allocate( const count_t& p_size )
    {
        fftw_complex* t_pointer;
        f_mutex.lock();
        t_pointer = fftw_alloc_complex( p_size );
        f_mutex.unlock();
        return t_pointer;
    }
    void fourier::free( fftw_complex* p_pointer )
    {
        f_mutex.lock();
        fftw_free( p_pointer );
        f_mutex.unlock();
        return;
    }

    fftw_plan fourier::forward( const count_t& p_size, fftw_complex* p_input, fftw_complex* p_output )
    {
        fftw_plan t_pointer;
        f_mutex.lock();
        t_pointer = fftw_plan_dft_1d( p_size, p_input, p_output, FFTW_FORWARD, FFTW_MEASURE );
        f_mutex.unlock();
        return t_pointer;
    }
    fftw_plan fourier::backward( const count_t& p_size, fftw_complex* p_input, fftw_complex* p_output )
    {
        fftw_plan t_pointer;
        f_mutex.lock();
        t_pointer = fftw_plan_dft_1d( p_size, p_input, p_output, FFTW_BACKWARD, FFTW_MEASURE );
        f_mutex.unlock();
        return t_pointer;
    }
    void fourier::destroy( fftw_plan p_pointer )
    {
        f_mutex.lock();
        fftw_destroy_plan( p_pointer );
        f_mutex.unlock();
        return;
    }

    void fourier::execute( fftw_plan p_plan )
    {
        fftw_execute( p_plan );
        return;
    }
    void fourier::execute( fftw_plan p_plan, fftw_complex* p_input, fftw_complex* p_output )
    {
        fftw_execute_dft( p_plan, p_input, p_output );
        return;
    }

}
