#include "cf_rf_power_transformer.hh"

#include <cmath>

namespace midge
{

    cf_rf_power_transformer::cf_rf_power_transformer() :
            f_impedance_ohm( 1. ),
            f_last( 0 ),
            f_nyquist( true ),
            f_in( NULL ),
            f_out( NULL ),
            f_norm( 1. )
    {
    }
    cf_rf_power_transformer::~cf_rf_power_transformer()
    {
    }

    void cf_rf_power_transformer::set_impedance_ohm( const real_t& p_impedance )
    {
        f_impedance_ohm = p_impedance;
        return;
    }
    const real_t& cf_rf_power_transformer::get_impedance_ohm() const
    {
        return f_impedance_ohm;
    }

    void cf_rf_power_transformer::start_transformer()
    {
        if( in< 0 >()->get_size() % 2 == 0 )
        {
            out< 0 >()->set_size( (in< 0 >()->get_size() / 2) + 1 );
            f_nyquist = true;
            f_last = (in< 0 >()->get_size() / 2);
        }
        else
        {
            out< 0 >()->set_size( (in< 0 >()->get_size() + 1) / 2 );
            f_nyquist = false;
            f_last = (in< 0 >()->get_size() + 1) / 2;
        }
        out< 0 >()->set_interval( in< 0 >()->get_interval() );

        f_in = in< 0 >()->raw();
        f_out = out< 0 >()->raw();
        f_norm = f_impedance_ohm * in< 0 >()->get_size();

        return;
    }
    void cf_rf_power_transformer::execute_transformer()
    {
        real_t t_power_watt;
        real_t t_power_dbm;

        t_power_watt = ( f_in[ 0 ][ 0 ] * f_in[ 0 ][ 0 ] + f_in[ 0 ][ 1 ] * f_in[ 0 ][ 1 ] ) / f_norm;
        t_power_dbm = 10. * log10( t_power_watt ) + 30.;
        f_out[ 0 ] = t_power_dbm;
        for( count_t t_index = 1; t_index < f_last; t_index++ )
        {
            t_power_watt = 2. * ( f_in[ t_index ][ 0 ] * f_in[ t_index ][ 0 ] + f_in[ t_index ][ 1 ] * f_in[ t_index ][ 1 ] ) / f_norm;
            t_power_dbm = 10. * log10( t_power_watt ) + 30.;
            f_out[ t_index ] = t_power_dbm;
        }
        if( f_nyquist == true )
        {
            t_power_watt = ( f_in[ f_last ][ 0 ] * f_in[ f_last ][ 0 ] + f_in[ f_last ][ 1 ] * f_in[ f_last ][ 1 ] ) / f_norm;
            t_power_dbm = 10. * log10( t_power_watt ) + 30.;
            f_out[ f_last ] = t_power_dbm;
        }

        out< 0 >()->set_center_time( in< 0 >()->get_center_time() );

        return;
    }
    void cf_rf_power_transformer::stop_transformer()
    {
        f_last = 0;
        f_nyquist = true;
        f_in = NULL;
        f_out = NULL;

        return;
    }

}
