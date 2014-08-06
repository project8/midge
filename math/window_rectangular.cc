#include "window_rectangular.hh"

#include <cmath>

namespace midge
{

    window_rectangular::window_rectangular()
    {
    }
    window_rectangular::~window_rectangular()
    {
    }

    void window_rectangular::fill( const count_t& p_size, real_t*& p_raw )
    {
        for( count_t t_index = 0; t_index < p_size; t_index++ )
        {
            p_raw[ t_index ] = 1.;
        }
        return;
    }

}
