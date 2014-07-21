#include "real_complex_transformer.hh"

namespace midge
{

    real_complex_transformer::real_complex_transformer() :
            f_mode( e_real ),
            f_size( 0 ),
            f_in_raw( NULL ),
            f_out_raw( NULL )
    {
    }
    real_complex_transformer::~real_complex_transformer()
    {
    }

    void real_complex_transformer::set_mode( const string& p_string )
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

        throw error() << "real complex transformer got mode <" << p_string << "> which may only be one of { real, imaginary }";
        return;
    }
    const real_complex_transformer::mode& real_complex_transformer::get_mode() const
    {
        return f_mode;
    }

    void real_complex_transformer::initialize_transformer()
    {
        if( input< 0 >()->get_size() != output< 0 >()->get_size() )
        {
            throw error() << "real complex transformer named <" << this->get_name() << "> must have matching input and output sizes\n" << "in was <" << input< 0 >()->get_size() << ">\n" << "out was <" << output< 0 >()->get_size() << ">";
        }

        f_size = input< 0 >()->get_size();
        f_in_raw = input< 0 >()->raw();
        f_out_raw = output< 0 >()->raw();

        return;
    }
    void real_complex_transformer::execute_transformer()
    {
        switch( f_mode )
        {
            case e_real :
                for( uint64_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_out_raw[ t_index ][ 0 ] = f_in_raw[ t_index ];
                    f_out_raw[ t_index ][ 1 ] = 0.;
                }
                return;

            case e_imaginary :
                for( uint64_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_out_raw[ t_index ][ 0 ] = 0.;
                    f_out_raw[ t_index ][ 1 ] = f_in_raw[ t_index ];
                }
                return;
        }
    }
}
