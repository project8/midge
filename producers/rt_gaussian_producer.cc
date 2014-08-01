#include "rt_gaussian_producer.hh"

#include "gsl/gsl_randist.h"

#include <cmath>

namespace midge
{

    rt_gaussian_producer::rt_gaussian_producer() :
            f_power_dbm( 0. ),
            f_impedance_ohm( 1. ),
            f_start_sec( 0. ),
            f_stop_sec( 1. ),
            f_seed( 51385 ),
            f_stride( 0 ),
            f_size( 0 ),
            f_interval( 1. ),
            f_amplitude( 0. ),
            f_start( 0 ),
            f_stop( 0 ),
            f_index( 0 ),
            f_rng( NULL ),
            f_out( NULL )
    {
    }
    rt_gaussian_producer::~rt_gaussian_producer()
    {
    }

    void rt_gaussian_producer::set_power_dbm( const real_t& p_power )
    {
        f_power_dbm = p_power;
        return;
    }
    const real_t& rt_gaussian_producer::get_power_dbm() const
    {
        return f_power_dbm;
    }

    void rt_gaussian_producer::set_impedance_ohm( const real_t& p_impedance )
    {
        f_impedance_ohm = p_impedance;
        return;
    }
    const real_t& rt_gaussian_producer::get_impedance_ohm() const
    {
        return f_impedance_ohm;
    }

    void rt_gaussian_producer::set_start_sec( const real_t& p_start )
    {
        f_start_sec = p_start;
        return;
    }
    const real_t& rt_gaussian_producer::get_start_sec() const
    {
        return f_start_sec;
    }

    void rt_gaussian_producer::set_stop_sec( const real_t& p_phase )
    {
        f_stop_sec = p_phase;
        return;
    }
    const real_t& rt_gaussian_producer::get_stop_sec() const
    {
        return f_stop_sec;
    }

    void rt_gaussian_producer::set_seed( const count_t& p_seed )
    {
        f_seed = p_seed;
        return;
    }
    const count_t& rt_gaussian_producer::get_seed() const
    {
        return f_seed;
    }

    void rt_gaussian_producer::set_stride( const count_t& p_stride )
    {
        f_stride = p_stride;
        return;
    }
    const count_t& rt_gaussian_producer::get_stride() const
    {
        return f_stride;
    }

    void rt_gaussian_producer::set_size( const count_t& p_size )
    {
        f_size = p_size;
        return;
    }
    const count_t& rt_gaussian_producer::get_size() const
    {
        return f_size;
    }

    void rt_gaussian_producer::set_interval( const real_t& p_interval )
    {
        f_interval = p_interval;
        return;
    }
    const real_t& rt_gaussian_producer::get_interval() const
    {
        return f_interval;
    }

    void rt_gaussian_producer::initialize_producer()
    {
        f_amplitude = sqrt( f_impedance_ohm ) * pow( 10., (f_power_dbm - 30.) / 20. );
        f_start = (count_t) (floor( f_start_sec / f_interval ));
        f_stop = (count_t) (ceil( f_stop_sec / f_interval ));

        f_rng = gsl_rng_alloc( gsl_rng_mt19937 );
        gsl_rng_set( f_rng, f_seed );

        return;
    }

    bool rt_gaussian_producer::start_producer()
    {
        out< 0 >()->set_size( f_size );
        out< 0 >()->set_interval( f_interval );

        f_out = out< 0 >()->raw();
        f_index = 0;

        return true;
    }

    bool rt_gaussian_producer::execute_producer()
    {
        count_t t_offset;
        if( f_stride < f_size )
        {
            t_offset = f_size - f_stride;
            for( count_t t_index = 0; t_index < f_size - f_stride; t_index++ )
            {
                f_out[ t_index ] = f_out[ t_index + f_stride ];
            }
        }
        else
        {
            t_offset = 0;
        }

        for( count_t t_index = t_offset; t_index < f_size; t_index++ )
        {
            if( (t_index + f_index >= f_start) && (t_index + f_index <= f_stop) )
            {
                f_out[ t_index ] = gsl_ran_gaussian( f_rng, f_amplitude );
            }
            else
            {
                f_out[ t_index ] = 0.;
            }
        }

        out< 0 >()->set_start_time( f_index * f_interval );
        f_index += f_stride;

        return true;
    }

    bool rt_gaussian_producer::stop_producer()
    {
        f_out = NULL;
        f_index = 0;

        return true;
    }

    void rt_gaussian_producer::finalize_producer()
    {
        f_amplitude = 0.;
        f_start = 0;
        f_stop = 0;
        f_index = 0;

        gsl_rng_free( f_rng );

        return;
    }

}
