#include "rt_rf_wigner_transformer.hh"

#include <cmath>

namespace midge
{

    rt_rf_wigner_transformer::rt_rf_wigner_transformer() :
            f_offset( 0 ),
            f_length( 0 ),
            f_in_size( 0 ),
            f_out_size( 0 ),
            f_under( 0 ),
            f_nyquist( 0 ),
            f_over( 0 ),
            f_norm( 1. ),
            f_in( NULL ),
            f_out( NULL ),
            f_signal( NULL ),
            f_transform( NULL ),
            f_analytic( NULL ),
            f_correlation( NULL ),
            f_distribution( NULL ),
            f_forward( NULL ),
            f_backward( NULL ),
            f_final( NULL )
    {
    }
    rt_rf_wigner_transformer::~rt_rf_wigner_transformer()
    {
    }

    void rt_rf_wigner_transformer::set_offset( const count_t& p_offset )
    {
        f_offset = p_offset;
        return;
    }
    const count_t& rt_rf_wigner_transformer::get_offset() const
    {
        return f_offset;
    }

    void rt_rf_wigner_transformer::set_length( const count_t& p_length )
    {
        f_length = p_length;
        return;
    }
    const count_t& rt_rf_wigner_transformer::get_length() const
    {
        return f_length;
    }

    bool rt_rf_wigner_transformer::start_transformer()
    {
        f_in_size = in< 0 >()->get_size();
        f_out_size = 2 * f_length + 2;
        if( f_in_size % 2 == 0 )
        {
            f_under = (f_in_size / 2) - 1;
            f_nyquist = (f_in_size / 2);
            f_over = (f_in_size / 2) + 1;
        }
        else
        {
            f_under = (f_in_size - 1) / 2;
            f_nyquist = 0;
            f_over = (f_in_size + 1) / 2;
        }
        f_norm = 2. / (f_in_size * f_in_size);

        out< 0 >()->set_size( f_out_size );
        out< 0 >()->set_interval( 1. / (2. * in< 0 >()->get_interval() * f_out_size) );
        f_in = in< 0 >()->raw();
        f_out = out< 0 >()->raw();

        f_signal = (fftw_complex*) (fftw_malloc( f_in_size * sizeof(fftw_complex) ));
        f_transform = (fftw_complex*) (fftw_malloc( f_in_size * sizeof(fftw_complex) ));
        f_analytic = (fftw_complex*) (fftw_malloc( f_in_size * sizeof(fftw_complex) ));
        f_correlation = (fftw_complex*) (fftw_malloc( f_out_size * sizeof(fftw_complex) ));
        f_distribution = (fftw_complex*) (fftw_malloc( f_out_size * sizeof(fftw_complex) ));
        f_forward = fftw_plan_dft_1d( f_in_size, f_signal, f_transform, FFTW_FORWARD, FFTW_MEASURE );
        f_backward = fftw_plan_dft_1d( f_in_size, f_transform, f_analytic, FFTW_BACKWARD, FFTW_MEASURE );
        f_final = fftw_plan_dft_1d( f_out_size, f_correlation, f_distribution, FFTW_FORWARD, FFTW_MEASURE );

        return true;
    }
    bool rt_rf_wigner_transformer::execute_transformer()
    {
        count_t t_index;

        // input to signal
        for( t_index = 0; t_index < f_in_size; t_index++ )
        {
            f_signal[ t_index ][ 0 ] = f_in[ t_index ];
            f_signal[ t_index ][ 1 ] = 0.;
        }

        // signal to transform
        fftw_execute( f_forward );

        // transform under hilbert transformation
        f_transform[ 0 ][ 0 ] = 1. * f_transform[ 0 ][ 0 ];
        f_transform[ 0 ][ 1 ] = 1. * f_transform[ 0 ][ 1 ];
        for( t_index = 1; t_index <= f_under; t_index++ )
        {
            f_transform[ t_index ][ 0 ] = 2. * f_transform[ t_index ][ 0 ];
            f_transform[ t_index ][ 1 ] = 2. * f_transform[ t_index ][ 1 ];
        }
        if( f_nyquist != 0 )
        {
            f_transform[ f_nyquist ][ 0 ] = 1. * f_transform[ f_nyquist ][ 0 ];
            f_transform[ f_nyquist ][ 1 ] = 1. * f_transform[ f_nyquist ][ 1 ];
        }
        for( t_index = f_over; t_index < f_in_size; t_index++ )
        {
            f_transform[ t_index ][ 0 ] = 0.;
            f_transform[ t_index ][ 1 ] = 0.;
        }

        // transform to analytic
        fftw_execute( f_backward );

        // analytic to correlation
        register real_t t_a;
        register real_t t_b;
        register real_t t_c;
        register real_t t_d;
        for( t_index = 0; t_index <= f_length; t_index++ )
        {
            t_a = f_analytic[ f_offset + t_index ][ 0 ];
            t_b = f_analytic[ f_offset + t_index ][ 1 ];
            t_c = f_analytic[ f_offset - t_index ][ 0 ];
            t_d = f_analytic[ f_offset - t_index ][ 1 ];

            f_correlation[ t_index ][ 0 ] = f_norm * (t_a * t_c + t_b * t_d);
            f_correlation[ t_index ][ 1 ] = f_norm * (t_b * t_c - t_a * t_d);
        }
        f_correlation[ f_length + 1 ][ 0 ] = 0.;
        f_correlation[ f_length + 1 ][ 1 ] = 0.;
        for( t_index = f_length + 2; t_index <= 2 * f_length + 1; t_index++ )
        {
            t_a = f_analytic[ f_offset + t_index - 2 * f_length - 2 ][ 0 ];
            t_b = f_analytic[ f_offset + t_index - 2 * f_length - 2 ][ 1 ];
            t_c = f_analytic[ f_offset - t_index + 2 * f_length + 2 ][ 0 ];
            t_d = f_analytic[ f_offset - t_index + 2 * f_length + 2 ][ 1 ];

            f_correlation[ t_index ][ 0 ] = f_norm * (t_a * t_c + t_b * t_d);
            f_correlation[ t_index ][ 1 ] = f_norm * (t_b * t_c - t_a * t_d);
        }

        // correlation to distribution
        fftw_execute( f_final );

        for( t_index = 0; t_index < f_out_size; t_index++ )
        {
            f_out[ t_index ] = f_distribution[ t_index ][ 0 ];
        }

        // update time
        out< 0 >()->set_center_time( in< 0 >()->get_start_time() + f_offset * in< 0 >()->get_interval() );

        return true;
    }
    bool rt_rf_wigner_transformer::stop_transformer()
    {
        f_in_size = 0;
        f_out_size = 0;
        f_under = 0;
        f_nyquist = 0;
        f_over = 0;
        f_norm = 1.;
        f_in = NULL;
        f_out = NULL;

        fftw_free( f_signal );
        f_signal = NULL;

        fftw_free( f_transform );
        f_transform = NULL;

        fftw_free( f_analytic );
        f_analytic = NULL;

        fftw_free( f_correlation );
        f_correlation = NULL;

        fftw_free( f_distribution );
        f_distribution = NULL;

        fftw_destroy_plan( f_forward );
        f_forward = NULL;

        fftw_destroy_plan( f_backward );
        f_backward = NULL;

        fftw_destroy_plan( f_final );
        f_final = NULL;

        return true;
    }

}
