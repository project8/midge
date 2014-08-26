#include "window.hh"

namespace midge
{

    window::window() :
            f_size( 0 ),
            f_raw( NULL ),
            f_sum( 0. ),
            f_square( 0. )
    {
    }
    window::~window()
    {
        if( f_raw != NULL )
        {
            delete[] f_raw;
        }
    }

    void window::set_size( const count_t& p_size )
    {
        if( f_size == p_size )
        {
            return;
        }
        f_size = p_size;

        if( f_raw != NULL )
        {
            delete[] f_raw;
        }
        f_raw = new real_t[ f_size ];

        fill( f_size, f_raw );

        f_sum = 0.;
        f_square = 0.;
        for( count_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_sum += f_raw[ t_index ];
            f_square += f_raw[ t_index ] * f_raw[ t_index ];
        }

        return;
    }
    const count_t& window::get_size() const
    {
        return f_size;
    }

    const real_t* window::raw() const
    {
        return f_raw;
    }
    const real_t& window::sum() const
    {
        return f_sum;
    }
    const real_t& window::square() const
    {
        return f_square;
    }

}
