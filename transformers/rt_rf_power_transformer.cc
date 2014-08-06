#include "rt_rf_power_transformer.hh"

#include <cmath>

namespace midge
{

    rt_rf_power_transformer::rt_rf_power_transformer() :
            f_impedance_ohm( 1. ),
            f_window( NULL ),
            f_in_size( 0 ),
            f_out_size( 0 ),
            f_last( 0 ),
            f_nyquist( 0 ),
            f_in( NULL ),
            f_in_interval( 0. ),
            f_in_time( 0. ),
            f_out( NULL ),
            f_out_interval( 0. ),
            f_out_time( 0. ),
            f_multiplier( NULL ),
            f_signal( NULL ),
            f_transform( NULL ),
            f_plan( NULL ),
            f_norm( 1. )
    {
    }
    rt_rf_power_transformer::~rt_rf_power_transformer()
    {
    }

    bool rt_rf_power_transformer::start_transformer()
    {
        f_in = in< 0 >()->raw();
        f_in_size = in< 0 >()->get_size();
        if( f_in_size % 2 == 0 )
        {
            f_out_size = (f_in_size / 2) + 1;
            f_last = (f_in_size / 2) - 1;
            f_nyquist = f_in_size / 2;
        }
        else
        {
            f_out_size = (f_in_size + 1) / 2;
            f_last = f_in_size / 2;
            f_nyquist = 0;
        }
        out< 0 >()->set_size( f_out_size );
        f_out = out< 0 >()->raw();

        f_in_interval = in< 0 >()->get_interval();
        f_out_interval = 1. / (f_in_size * f_in_interval);
        out< 0 >()->set_interval( f_out_interval );

        if( f_window == NULL )
        {
            throw error() << "rt rf power transformer window unset";
        }
        f_window->initialize( f_in_size );
        f_multiplier = f_window->raw();

        f_signal = (fftw_complex*) (fftw_malloc( f_in_size * sizeof(fftw_complex) ));
        f_transform = (fftw_complex*) (fftw_malloc( f_in_size * sizeof(fftw_complex) ));
        f_plan = fftw_plan_dft_1d( f_in_size, f_signal, f_transform, FFTW_FORWARD, FFTW_MEASURE );
        f_norm = 1. / (f_impedance_ohm * f_window->sum() * f_window->sum());

        return true;
    }
    bool rt_rf_power_transformer::execute_transformer()
    {
        // real signal to complex signal
        for( count_t t_index = 0; t_index < f_in_size; t_index++ )
        {
            f_signal[ t_index ][ 0 ] = f_in[ t_index ] * f_multiplier[ t_index ];
            f_signal[ t_index ][ 1 ] = 0.;
        }

        // complex signal to complex spectrum
        fftw_execute( f_plan );

        // complex spectrum to power spectrum
        real_t t_power_watt = (f_transform[ 0 ][ 0 ] * f_transform[ 0 ][ 0 ] + f_transform[ 0 ][ 1 ] * f_transform[ 0 ][ 1 ]) * f_norm;
        real_t t_power_dbm = 10. * log10( t_power_watt ) + 30.;
        f_out[ 0 ] = t_power_watt;
        for( count_t t_index = 1; t_index <= f_last; t_index++ )
        {
            t_power_watt = 2. * (f_transform[ t_index ][ 0 ] * f_transform[ t_index ][ 0 ] + f_transform[ t_index ][ 1 ] * f_transform[ t_index ][ 1 ]) * f_norm;
            t_power_dbm = 10. * log10( t_power_watt ) + 30.;
            f_out[ t_index ] = t_power_watt;
        }
        if( f_nyquist != 0 )
        {
            t_power_watt = (f_transform[ f_nyquist ][ 0 ] * f_transform[ f_nyquist ][ 0 ] + f_transform[ f_nyquist ][ 1 ] * f_transform[ f_nyquist ][ 1 ]) * f_norm;
            t_power_dbm = 10. * log10( t_power_watt ) + 30.;
            f_out[ f_nyquist ] = t_power_watt;
        }

        // update time
        f_in_time = in< 0 >()->get_time();
        f_out_time = f_in_time + .5 * f_in_size * f_in_interval;
        out< 0 >()->set_time( f_out_time );

        return true;
    }
    bool rt_rf_power_transformer::stop_transformer()
    {
        f_in_size = 0;
        f_out_size = 0;
        f_last = 0;
        f_nyquist = 0;

        f_in = NULL;
        f_in_interval = 1.;
        f_in_time = 0.;

        f_out = NULL;
        f_out_interval = 1.;
        f_out_time = 0.;

        delete f_window;
        f_multiplier = NULL;

        fftw_free( f_signal );
        f_signal = NULL;

        fftw_free( f_transform );
        f_transform = NULL;

        fftw_destroy_plan( f_plan );
        f_plan = NULL;

        f_norm = 1.;

        return true;
    }

}
