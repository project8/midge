#include "cf_ct_fourier_transformer.hh"

#include <cmath>

namespace midge
{

    cf_ct_fourier_transformer::cf_ct_fourier_transformer() :
            f_size( 0 ),
            f_in( NULL ),
            f_in_interval( 0. ),
            f_in_time( 0. ),
            f_out( NULL ),
            f_out_interval( 0. ),
            f_out_time( 0. ),
            f_plan( NULL ),
            f_norm( 1. )
    {
    }
    cf_ct_fourier_transformer::~cf_ct_fourier_transformer()
    {
    }

    bool cf_ct_fourier_transformer::start_transformer()
    {
        f_in = in< 0 >()->raw();
        f_size = in< 0 >()->get_size();
        out< 0 >()->set_size( f_size );
        f_out = out< 0 >()->raw();

        f_in_interval = in< 0 >()->get_interval();
        f_out_interval = 1. / (f_size * f_in_interval);
        out< 0 >()->set_interval( f_out_interval );

        f_plan = fftw_plan_dft_1d( f_size, f_in, f_out, FFTW_BACKWARD, FFTW_MEASURE );
        f_norm = sqrt( (real_t) (f_size) );

        return true;
    }
    bool cf_ct_fourier_transformer::execute_transformer()
    {
        fftw_execute( f_plan );

        for( count_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_out[ t_index ][ 0 ] /= f_norm;
            f_out[ t_index ][ 1 ] /= f_norm;
        }

        f_in_time = in< 0 >()->get_time();
        f_out_time = f_in_time - .5 * f_size * f_out_interval;
        out< 0 >()->set_time( f_out_time );

        return true;
    }
    bool cf_ct_fourier_transformer::stop_transformer()
    {
        f_size = 0;

        f_in = NULL;
        f_in_interval = 1.;
        f_in_time = 0.;

        f_out = NULL;
        f_out_interval = 1.;
        f_out_time = 0.;

        fftw_destroy_plan( f_plan );
        f_plan = NULL;

        f_norm = 1.;

        return true;
    }

}
