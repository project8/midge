#include "complex_real_transformer.hh"

#include <cmath>

namespace midge
{

    complex_real_transformer::complex_real_transformer() :
            f_mode( e_real ),
            f_size( 0 ),
            f_in_raw( NULL ),
            f_out_raw( NULL )
    {
    }
    complex_real_transformer::~complex_real_transformer()
    {
    }

    void complex_real_transformer::set_mode( const string& p_string )
    {
        if( p_string == string( "real" ) )
        {
            f_mode = e_real;
            return;
        }

        if( p_string == string( "imaginary" ) )
        {
            f_mode = e_imaginary;
            return;
        }

        if( p_string == string( "modulus" ) )
        {
            f_mode = e_modulus;
            return;
        }

        if( p_string == string( "argument" ) )
        {
            f_mode = e_argument;
            return;
        }

        throw error() << "complex real transformer got mode <" << p_string << "> which may only be one of { real, imaginary, modulus, argument }";
        return;
    }
    const complex_real_transformer::mode& complex_real_transformer::get_mode() const
    {
        return f_mode;
    }

    void complex_real_transformer::initialize_transformer()
    {
        if( input< 0 >()->get_size() != output< 0 >()->get_size() )
        {
            throw error() << "complex real transformer named <" << this->get_name() << "> must have matching input and output sizes";
        }

        f_size = input< 0 >()->get_size();
        f_in_raw = input< 0 >()->raw();
        f_out_raw = output< 0 >()->raw();

        return;
    }
    void complex_real_transformer::execute_transformer()
    {
        switch( f_mode )
        {
            case e_real :
                for( uint64_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_out_raw[ t_index ] = f_in_raw[ t_index ][ 0 ];
                }
                return;

            case e_imaginary :
                for( uint64_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_out_raw[ t_index ] = f_in_raw[ t_index ][ 1 ];
                }
                return;

            case e_modulus :
                for( uint64_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_out_raw[ t_index ] = sqrt( f_in_raw[ t_index ][ 0 ] * f_in_raw[ t_index ][ 0 ] + f_in_raw[ t_index ][ 1 ] * f_in_raw[ t_index ][ 1 ] );
                }
                return;

            case e_argument :
                for( uint64_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_out_raw[ t_index ] = atan2( f_in_raw[ t_index ][ 1 ], f_in_raw[ t_index ][ 0 ] );
                }
                return;
        }

    }
}
