#include "complex_backward_fourier_transformer.hh"

namespace midge
{

    complex_backward_fourier_transformer::complex_backward_fourier_transformer() :
            f_plan()
    {
    }
    complex_backward_fourier_transformer::~complex_backward_fourier_transformer()
    {
    }

    void complex_backward_fourier_transformer::initialize_transformer()
    {
        if( input< 0 >() == NULL )
        {
            //todo: throw here
        }

        if( output< 0 >() == NULL )
        {
            //todo: throw here
        }

        if( input< 0 >()->get_size() != output< 0 >()->get_size() )
        {
            //todo: throw here
        }

        fftw_plan_dft_1d( input< 0 >()->get_size(), input< 0 >()->data(), output< 0 >()->data(), FFTW_BACKWARD, FFTW_MEASURE );

        return;
    }
    void complex_backward_fourier_transformer::deinitialize_transformer()
    {
        fftw_destroy_plan( f_plan );

        return;
    }
    void complex_backward_fourier_transformer::update_transformer()
    {
        fftw_execute( f_plan );

        return;
    }

}
