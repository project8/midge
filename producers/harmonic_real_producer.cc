#include "harmonic_real_producer.hh"

#include <cmath>

namespace midge
{

    harmonic_real_producer::harmonic_real_producer() :
        f_amplitude( 1. ),
        f_frequency( .1 ),
        f_phase( 0. ),
        f_internal( 0. )
    {
    }
    harmonic_real_producer::~harmonic_real_producer()
    {
    }

    void harmonic_real_producer::set_amplitude( const real_t& p_amplitude )
    {
        f_amplitude = p_amplitude;
        return;
    }
    const real_t& harmonic_real_producer::get_amplitude() const
    {
        return f_amplitude;
    }

    void harmonic_real_producer::set_frequency( const real_t& p_frequency )
    {
        f_frequency = p_frequency;
        return;
    }
    const real_t& harmonic_real_producer::get_frequency() const
    {
        return f_frequency;
    }

    void harmonic_real_producer::set_phase( const real_t& p_phase )
    {
        f_phase = p_phase;
        return;
    }
    const real_t& harmonic_real_producer::get_phase() const
    {
        return f_phase;
    }

    void harmonic_real_producer::execute_producer()
    {
        uint64_t t_size = output< 0 >()->get_size();
        real_t* t_data = output< 0 >()->data();

        for( uint64_t t_index = 0; t_index < t_size; t_index++ )
        {
            t_data[t_index] = f_amplitude * cos( f_frequency * t_index + f_phase + f_internal );
        }

        f_internal = f_frequency * t_size;

        return;
    }

}
