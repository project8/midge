#include "ct_rt_transformer.hh"

#include <cmath>

namespace midge
{

    ct_rt_transformer::ct_rt_transformer() :
            f_mode( e_real ),
            f_size( 0 ),
            f_in( NULL ),
            f_out( NULL )
    {
    }
    ct_rt_transformer::~ct_rt_transformer()
    {
    }

    void ct_rt_transformer::set_mode( const string& p_string )
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

        throw error() << "ct->rt transformer got mode <" << p_string << "> which may only be one of { real, imaginary, modulus, argument }";
        return;
    }
    const ct_rt_transformer::mode& ct_rt_transformer::get_mode() const
    {
        return f_mode;
    }

    void ct_rt_transformer::start_transformer()
    {
        out< 0 >()->set_size( in< 0 >()->get_size() );
        out< 0 >()->set_interval( in< 0 >()->get_interval() );

        f_size = in< 0 >()->get_size();
        f_in = in< 0 >()->raw();
        f_out = out< 0 >()->raw();

        return;
    }
    void ct_rt_transformer::execute_transformer()
    {
        switch( f_mode )
        {
            case e_real :
                for( count_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_out[ t_index ] = f_in[ t_index ][ 0 ];
                }
                break;

            case e_imaginary :
                for( count_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_out[ t_index ] = f_in[ t_index ][ 1 ];
                }
                break;

            case e_modulus :
                for( count_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_out[ t_index ] = sqrt( f_in[ t_index ][ 0 ] * f_in[ t_index ][ 0 ] + f_in[ t_index ][ 1 ] * f_in[ t_index ][ 1 ] );
                }
                break;

            case e_argument :
                for( count_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_out[ t_index ] = atan2( f_in[ t_index ][ 1 ], f_in[ t_index ][ 0 ] );
                }
                break;
        }

        out< 0 >()->set_start_time( in< 0 >()->get_start_time() );

        return;
    }
    void ct_rt_transformer::stop_transformer()
    {
        f_size = 0;
        f_in = NULL;
        f_out = NULL;

        return;
    }

}
