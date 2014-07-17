#include "complex_real_transformer.hh"

#include <cmath>

namespace midge
{

    complex_real_transformer::complex_real_transformer() :
            f_mode( e_real ),
            f_size( 0 ),
            f_input( NULL ),
            f_output( NULL )
    {
    }
    complex_real_transformer::~complex_real_transformer()
    {
    }

    void complex_real_transformer::set_mode( const mode& p_mode )
    {
        f_mode = p_mode;
        return;
    }
    const complex_real_transformer::mode& complex_real_transformer::get_mode() const
    {
        return f_mode;
    }

    void complex_real_transformer::initialize_transformer()
    {
        if( input< 0 >()->size() != output< 0 >()->size() )
        {
            throw error() << "complex real transformer named <" << this->get_name() << "> must have matching input and output sizes";
        }

        f_size = input< 0 >()->size();
        f_input = input< 0 >()->data();
        f_output = output< 0 >()->data();

        return;
    }
    void complex_real_transformer::execute_transformer()
    {
        switch( f_mode )
        {
            case e_real :
                for( uint64_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_output[ t_index ] = f_input[ t_index ][ 0 ];
                }
                return;

            case e_imaginary :
                for( uint64_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_output[ t_index ] = f_input[ t_index ][ 1 ];
                }
                return;

            case e_modulus :
                for( uint64_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_output[ t_index ] = sqrt( f_input[ t_index ][ 0 ] * f_input[ t_index ][ 0 ] + f_input[ t_index ][ 1 ] * f_input[ t_index ][ 1 ] );
                }
                return;

            case e_argument :
                for( uint64_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_output[ t_index ] = atan2( f_input[ t_index ][ 1 ], f_input[ t_index ][ 0 ] );
                }
                return;
        }

    }
}
