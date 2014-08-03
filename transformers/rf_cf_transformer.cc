#include "rf_cf_transformer.hh"

namespace midge
{

    rf_cf_transformer::rf_cf_transformer() :
            f_mode( e_real ),
            f_size( 0 ),
            f_in( NULL ),
            f_out( NULL )
    {
    }
    rf_cf_transformer::~rf_cf_transformer()
    {
    }

    void rf_cf_transformer::set_mode( const string& p_string )
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

        throw error() << "rt->ct transformer got mode <" << p_string << "> which may only be one of { real, imaginary }";
        return;
    }
    const rf_cf_transformer::mode& rf_cf_transformer::get_mode() const
    {
        return f_mode;
    }

    bool rf_cf_transformer::start_transformer()
    {
        out< 0 >()->set_size( in< 0 >()->get_size() );
        out< 0 >()->set_interval( in< 0 >()->get_interval() );

        f_size = in< 0 >()->get_size();
        f_in = in< 0 >()->raw();
        f_out = out< 0 >()->raw();

        return true;
    }
    bool rf_cf_transformer::execute_transformer()
    {
        switch( f_mode )
        {
            case e_real :
                for( count_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_out[ t_index ][ 0 ] = f_in[ t_index ];
                    f_out[ t_index ][ 1 ] = 0.;
                }
                break;

            case e_imaginary :
                for( count_t t_index = 0; t_index < f_size; t_index++ )
                {
                    f_out[ t_index ][ 0 ] = 0.;
                    f_out[ t_index ][ 1 ] = f_in[ t_index ];
                }
                break;
        }

        out< 0 >()->set_time( in< 0 >()->get_time() );

        return true;
    }
    bool rf_cf_transformer::stop_transformer()
    {
        f_size = 0;
        f_in = NULL;
        f_out = NULL;

        return true;
    }


}
