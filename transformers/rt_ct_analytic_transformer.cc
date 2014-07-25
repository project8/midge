#include "rt_ct_analytic_transformer.hh"

#include <cmath>

namespace midge
{

    rt_ct_analytic_transformer::rt_ct_analytic_transformer() :
            f_size( 0 ),
            f_under( 0 ),
            f_nyquist( 0 ),
            f_over( 0 ),
            f_in( NULL ),
            f_out( NULL ),
            f_signal( NULL ),
            f_transform( NULL ),
            f_analytic( NULL ),
            f_forward( NULL ),
            f_backward( NULL ),
            f_norm( 1. )
    {
    }
    rt_ct_analytic_transformer::~rt_ct_analytic_transformer()
    {
    }

    bool rt_ct_analytic_transformer::start_transformer()
    {
        f_size = in< 0 >()->get_size();
        if( f_size % 2 == 0 )
        {
            f_under = (f_size / 2) - 1;
            f_nyquist = (f_size / 2);
            f_over = (f_size / 2) + 1;
        }
        else
        {
            f_under = (f_size - 1) / 2;
            f_nyquist = 0;
            f_over = (f_size + 1) / 2;
        }
        out< 0 >()->set_size( f_size );
        out< 0 >()->set_interval( 1. / (in< 0 >()->get_interval() * f_size) );

        f_in = in< 0 >()->raw();
        f_out = out< 0 >()->raw();

        f_signal = (fftw_complex*) (fftw_malloc( f_size * sizeof(fftw_complex) ));
        f_transform = (fftw_complex*) (fftw_malloc( f_size * sizeof(fftw_complex) ));
        f_analytic = (fftw_complex*) (fftw_malloc( f_size * sizeof(fftw_complex) ));
        f_forward = fftw_plan_dft_1d( f_size, f_signal, f_transform, FFTW_FORWARD, FFTW_MEASURE );
        f_backward = fftw_plan_dft_1d( f_size, f_transform, f_analytic, FFTW_BACKWARD, FFTW_MEASURE );

        f_norm = sqrt( f_size );

        return true;
    }
    bool rt_ct_analytic_transformer::execute_transformer()
    {
        count_t t_index;

        // input to complex signal
        for( t_index = 0; t_index < f_size; t_index++ )
        {
            f_signal[ t_index ][ 0 ] = f_in[ t_index ];
            f_signal[ t_index ][ 1 ] = 0.;
        }

        // complex signal to complex spectrum
        fftw_execute( f_forward );

        // complex spectrum under hilbert transformation
        f_transform[ 0 ][ 0 ] = 1. * f_transform[ 0 ][ 0 ] / f_norm;
        f_transform[ 0 ][ 1 ] = 1. * f_transform[ 0 ][ 1 ] / f_norm;
        for( t_index = 1; t_index <= f_under; t_index++ )
        {
            f_transform[ t_index ][ 0 ] = 2. * f_transform[ t_index ][ 0 ] / f_norm;
            f_transform[ t_index ][ 1 ] = 2. * f_transform[ t_index ][ 1 ] / f_norm;
        }
        if( f_nyquist != 0 )
        {
            f_transform[ f_nyquist ][ 0 ] = 1. * f_transform[ f_nyquist ][ 0 ] / f_norm;
            f_transform[ f_nyquist ][ 1 ] = 1. * f_transform[ f_nyquist ][ 1 ] / f_norm;
        }
        for( t_index = f_over; t_index < f_size; t_index++ )
        {
            f_transform[ t_index ][ 0 ] = 0. / f_norm;
            f_transform[ t_index ][ 1 ] = 0. / f_norm;
        }

        // complex spectrum to analytic signal
        fftw_execute( f_backward );

        // analytic signal to output
        for( t_index = 0; t_index < f_size; t_index++ )
        {
            f_out[ t_index ][ 0 ] = f_analytic[ t_index ][ 0 ] / f_norm;
            f_out[ t_index ][ 1 ] = f_analytic[ t_index ][ 1 ] / f_norm;
        }

        // update time
        out< 0 >()->set_start_time( in< 0 >()->get_start_time() );

        return true;
    }
    bool rt_ct_analytic_transformer::stop_transformer()
    {
        f_size = 0;
        f_under = 0;
        f_nyquist = 0;
        f_over = 0;
        f_in = NULL;
        f_out = NULL;

        fftw_free( f_signal );
        f_signal = NULL;

        fftw_free( f_transform );
        f_transform = NULL;

        fftw_free( f_analytic );
        f_analytic = NULL;

        fftw_destroy_plan( f_forward );
        f_forward = NULL;

        fftw_destroy_plan( f_backward );
        f_backward = NULL;

        f_norm = 1.;

        return true;
    }

}
