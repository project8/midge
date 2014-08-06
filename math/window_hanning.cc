#include "window_hanning.hh"

#include <cmath>

namespace midge
{

    window_hanning::window_hanning()
    {
    }
    window_hanning::~window_hanning()
    {
    }

    void window_hanning::fill( const count_t& p_size, real_t*& p_raw )
    {
        real_t t_real_size = (real_t) (p_size);
        real_t t_real_index;
        real_t t_scale = 2. * M_PI / (t_real_size - 1.);
        for( count_t t_index = 0; t_index < p_size; t_index++ )
        {
            t_real_index = (real_t) (t_index);
            p_raw[ t_index ] = .5 - .5 * cos( t_scale * t_real_index );
        }
        return;
    }
}
