#include "rt_harmonic_producer.hh"

#include <cmath>

namespace midge
{

    rt_harmonic_producer::rt_harmonic_producer() :
            f_power_dbm( 0. ),
            f_impedance_ohm( 1. ),
            f_frequency_hz( 1. ),
            f_phase_deg( 0. ),
            f_start_sec( 0. ),
            f_stop_sec( 1. ),
            f_stride( 0 ),
            f_size( 0 ),
            f_interval( 1. ),
            f_amplitude( 0. ),
            f_frequency( 0. ),
            f_phase( 0. ),
            f_start( 0 ),
            f_stop( 0 ),
            f_index( 0 ),
            f_out( NULL )
    {
    }
    rt_harmonic_producer::~rt_harmonic_producer()
    {
    }

    void rt_harmonic_producer::set_power_dbm( const real_t& p_power )
    {
        f_power_dbm = p_power;
        return;
    }
    const real_t& rt_harmonic_producer::get_power_dbm() const
    {
        return f_power_dbm;
    }

    void rt_harmonic_producer::set_impedance_ohm( const real_t& p_impedance )
    {
        f_impedance_ohm = p_impedance;
        return;
    }
    const real_t& rt_harmonic_producer::get_impedance_ohm() const
    {
        return f_impedance_ohm;
    }

    void rt_harmonic_producer::set_frequency_hz( const real_t& p_frequency )
    {
        f_frequency_hz = p_frequency;
        return;
    }
    const real_t& rt_harmonic_producer::get_frequency_hz() const
    {
        return f_frequency_hz;
    }

    void rt_harmonic_producer::set_phase_deg( const real_t& p_phase )
    {
        f_phase_deg = p_phase;
        return;
    }
    const real_t& rt_harmonic_producer::get_phase_deg() const
    {
        return f_phase_deg;
    }

    void rt_harmonic_producer::set_start_sec( const real_t& p_start )
    {
        f_start_sec = p_start;
        return;
    }
    const real_t& rt_harmonic_producer::get_start_sec() const
    {
        return f_start_sec;
    }

    void rt_harmonic_producer::set_stop_sec( const real_t& p_phase )
    {
        f_stop_sec = p_phase;
        return;
    }
    const real_t& rt_harmonic_producer::get_stop_sec() const
    {
        return f_stop_sec;
    }

    void rt_harmonic_producer::set_stride( const count_t& p_stride )
    {
        f_stride = p_stride;
        return;
    }
    const count_t& rt_harmonic_producer::get_stride() const
    {
        return f_stride;
    }

    void rt_harmonic_producer::set_size( const count_t& p_size )
    {
        f_size = p_size;
        return;
    }
    const count_t& rt_harmonic_producer::get_size() const
    {
        return f_size;
    }

    void rt_harmonic_producer::set_interval( const real_t& p_interval )
    {
        f_interval = p_interval;
        return;
    }
    const real_t& rt_harmonic_producer::get_interval() const
    {
        return f_interval;
    }

    void rt_harmonic_producer::initialize_producer()
    {
        f_amplitude = sqrt( 2. * f_impedance_ohm ) * pow( 10., (f_power_dbm - 30.) / 20. );
        f_frequency = f_interval * f_frequency_hz;
        f_phase = ( M_PI / 180.) * f_phase_deg;
        f_start = (count_t) (floor( f_start_sec / f_interval ));
        f_stop = (count_t) (ceil( f_stop_sec / f_interval ));
        f_index = 0;

        return;
    }

    bool rt_harmonic_producer::start_producer()
    {
        out< 0 >()->set_size( f_size );
        out< 0 >()->set_interval( f_interval );
        f_out = out< 0 >()->raw();

        return true;
    }

    bool rt_harmonic_producer::execute_producer()
    {
        for( count_t t_index = 0; t_index < f_size; t_index++ )
        {
            if( (f_index + t_index >= f_start) && (f_index + t_index <= f_stop) )
            {
                f_out[ t_index ] = f_amplitude * cos( 2. * M_PI * f_frequency * (t_index + f_index - f_start) + f_phase );
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

    bool rt_harmonic_producer::stop_producer()
    {
        f_out = NULL;

        return true;
    }

    void rt_harmonic_producer::finalize_producer()
    {
        f_amplitude = 0.;
        f_frequency = 0.;
        f_phase = 0.;
        f_start = 0.;
        f_stop = 0.;
        f_index = 0;

        return;
    }

}
