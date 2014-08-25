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
        finalize();
    }

    void window::initialize( const count_t& p_size )
    {
        if( f_size != p_size )
        {
            f_size = p_size;

            finalize();

            f_raw = new real_t[ f_size ];

            fill( f_size, f_raw );

            for( count_t t_index = 0; t_index < f_size; t_index++ )
            {
                f_sum += f_raw[ t_index ];
                f_square += f_raw[ t_index ] * f_raw[ t_index ];
            }
        }

        return;
    }
    void window::finalize()
    {
        if( f_raw != NULL )
        {
            delete[] f_raw;
            f_raw = NULL;
        }

        f_sum = 0.;
        f_square = 0.;

        return;
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
