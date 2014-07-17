#include "real_harmonic_producer.hh"

#include <cmath>

namespace midge
{

    real_harmonic_producer::real_harmonic_producer() :
            f_amplitude( 1. ),
            f_frequency( .1 ),
            f_phase( 0. ),
            f_internal( 0. ),
            f_data( NULL ),
            f_size( 0 )
    {
    }
    real_harmonic_producer::~real_harmonic_producer()
    {
    }

    void real_harmonic_producer::set_amplitude( const real_t& p_amplitude )
    {
        f_amplitude = p_amplitude;
        return;
    }
    const real_t& real_harmonic_producer::get_amplitude() const
    {
        return f_amplitude;
    }

    void real_harmonic_producer::set_frequency( const real_t& p_frequency )
    {
        f_frequency = p_frequency;
        return;
    }
    const real_t& real_harmonic_producer::get_frequency() const
    {
        return f_frequency;
    }

    void real_harmonic_producer::set_phase( const real_t& p_phase )
    {
        f_phase = p_phase;
        return;
    }
    const real_t& real_harmonic_producer::get_phase() const
    {
        return f_phase;
    }

    void real_harmonic_producer::initialize_producer()
    {
        f_size = output< 0 >()->size();
        f_data = output< 0 >()->data();
        return;
    }

    void real_harmonic_producer::execute_producer()
    {
        for( uint64_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_data[ t_index ] = f_amplitude * cos( 2. * M_PI * f_frequency * t_index + f_phase + f_internal );
        }

        f_internal += 2. * M_PI * f_frequency * f_size;

        return;
    }

}
