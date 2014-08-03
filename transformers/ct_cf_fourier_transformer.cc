#include "ct_cf_fourier_transformer.hh"

#include <cmath>

namespace midge
{

    ct_cf_fourier_transformer::ct_cf_fourier_transformer() :
            f_size( 0 ),
            f_in( NULL ),
            f_out( NULL ),
            f_plan(),
            f_norm( 1. )
    {
    }
    ct_cf_fourier_transformer::~ct_cf_fourier_transformer()
    {
    }

    bool ct_cf_fourier_transformer::start_transformer()
    {
        out< 0 >()->set_size( in< 0 >()->get_size() );
        out< 0 >()->set_interval( 1. / (in< 0 >()->get_size() * in< 0 >()->get_interval()) );

        f_size = in< 0 >()->get_size();
        f_in = in< 0 >()->raw();
        f_out = out< 0 >()->raw();
        f_plan = fftw_plan_dft_1d( f_size, f_in, f_out, FFTW_FORWARD, FFTW_MEASURE );
        f_norm = sqrt( (real_t) (f_size) );

        return true;
    }
    bool ct_cf_fourier_transformer::execute_transformer()
    {
        fftw_execute( f_plan );

        for( count_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_out[ t_index ][ 0 ] /= f_norm;
            f_out[ t_index ][ 1 ] /= f_norm;
        }
        out< 0 >()->set_time( in< 0 >()->get_time() );

        return true;
    }
    bool ct_cf_fourier_transformer::stop_transformer()
    {
        f_size = 0;
        f_in = NULL;
        f_out = NULL;
        fftw_destroy_plan( f_plan );
        f_norm = 1.;

        return true;
    }

}
