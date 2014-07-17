#include "real_complex_transformer.hh"

namespace midge
{

    real_complex_transformer::real_complex_transformer() :
            f_mode( e_real ),
            f_size( 0 ),
            f_input( NULL ),
            f_output( NULL )
    {
    }
    real_complex_transformer::~real_complex_transformer()
    {
    }

    void real_complex_transformer::set_mode( const mode& p_mode )
    {
        f_mode = p_mode;
        return;
    }
    const real_complex_transformer::mode& real_complex_transformer::get_mode() const
    {
        return f_mode;
    }

    void real_complex_transformer::initialize_transformer()
    {
        if( input< 0 >()->size() != output< 0 >()->size() )
        {
            throw error() << "real complex transformer named <" << this->get_name() << "> must have matching input and output sizes";
        }

        f_size = input< 0 >()->size();
        f_input = input< 0 >()->data();
        f_output = output< 0 >()->data();

        return;
    }
    void real_complex_transformer::execute_transformer()
    {
        switch( f_mode )
        {
            case e_real:
                for( uint64_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_output[ t_index ][ 0 ] = f_input[ t_index ];
                }
                return;

            case e_imaginary:
                for( uint64_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_output[ t_index ][ 1 ] = f_input[ t_index ];
                }
                return;
        }
    }
}
