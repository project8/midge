#include "window_blackman.hh"

#include <cmath>

namespace midge
{

    window_blackman::window_blackman()
    {
    }
    window_blackman::~window_blackman()
    {
    }

    void window_blackman::fill( const count_t& p_size, real_t*& p_raw )
    {
        real_t t_real_size = (real_t) (p_size);
        real_t t_real_index;
        real_t t_scale_two = 2. * M_PI / (t_real_size - 1.);
        real_t t_scale_four = 4. * M_PI / (t_real_size - 1.);
        real_t t_scale_six = 6. * M_PI / (t_real_size - 1.);
        for( index_t t_index = 0; t_index < p_size; t_index++ )
        {
            t_real_index = (real_t) (t_index);
            p_raw[ t_index ] = .35875 - .48829 * cos( t_scale_two * t_real_index ) + .14128 * cos( t_scale_four * t_real_index ) - .01168 * cos( t_scale_six * t_real_index );
        }
        return;
    }
}
