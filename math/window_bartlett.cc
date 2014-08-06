#include "window_bartlett.hh"

#include <cmath>

namespace midge
{

    window_bartlett::window_bartlett()
    {
    }
    window_bartlett::~window_bartlett()
    {
    }

    void window_bartlett::fill( const count_t& p_size, real_t*& p_raw )
    {
        real_t t_real_size = (real_t) (p_size);
        real_t t_real_index;
        real_t t_center = .5 * (t_real_size - 1.);
        real_t t_scale = .5 * (t_real_size + 1.);
        for( count_t t_index = 0; t_index < p_size; t_index++ )
        {
            t_real_index = (real_t) (t_index);
            p_raw[ t_index ] = 1. - fabs( (t_real_index - t_center) / t_scale );
        }
        return;
    }
}
