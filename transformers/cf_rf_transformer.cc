#include "cf_rf_transformer.hh"

#include <cmath>

namespace midge
{

    cf_rf_transformer::cf_rf_transformer() :
            f_mode( "real" ),
            f_length( 10 )
    {
    }
    cf_rf_transformer::~cf_rf_transformer()
    {
    }

    void cf_rf_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );
        out_buffer< 0 >().set_name( get_name() );

        return;
    }

    void cf_rf_transformer::execute()
    {
        count_t t_index;

        state_t t_in_state;
        const cf_data* t_in_data;
        const complex_t* t_in_raw;

        rf_data* t_out_data;
        real_t* t_out_raw;

        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;
        real_t t_frequency_interval;
        count_t t_frequency_index;

        void (cf_rf_transformer::*t_converter)( const complex_t&, real_t& );

        if( f_mode == string( "real" ) )
        {
            t_converter = &cf_rf_transformer::real;
        }
        else if( f_mode == string( "imaginary" ) )
        {
            t_converter = &cf_rf_transformer::imaginary;
        }
        else if( f_mode == string( "modulus" ) )
        {
            t_converter = &cf_rf_transformer::modulus;
        }
        else if( f_mode == string( "argument" ) )
        {
            t_converter = &cf_rf_transformer::argument;
        }
        else
        {
            throw error() << "ct rt transformer <" << get_name() << "> got bad mode string <" << f_mode << ">";
            return;
        }

        while( true )
        {
            in_stream< 0 >()++;
            t_in_state = in_stream< 0 >().state();
            t_in_data = in_stream< 0 >().data();
            t_out_data = out_stream< 0 >().data();

            if( t_in_state == stream::s_start )
            {
                t_size = t_in_data->get_size();
                t_time_interval = t_in_data->get_time_interval();
                t_time_index = t_in_data->get_time_index();
                t_frequency_interval = t_in_data->get_frequency_interval();
                t_frequency_index = t_in_data->get_frequency_index();

                t_out_data->set_size( t_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index );
                t_out_data->set_frequency_interval( t_frequency_interval );
                t_out_data->set_frequency_index( t_frequency_index );

                out_stream< 0 >().state( stream::s_start );
                t_index = out_stream< 0 >()++;

                continue;
            }
            if( t_in_state == stream::s_run )
            {
                t_time_index = t_in_data->get_time_index();
                t_frequency_index = t_in_data->get_frequency_index();
                t_in_raw = t_in_data->raw();

                t_out_data->set_size( t_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index );
                t_out_data->set_frequency_interval( t_frequency_interval );
                t_out_data->set_frequency_index( t_frequency_index );
                t_out_raw = t_out_data->raw();

                for( t_index = 0; t_index < t_size; t_index++ )
                {
                    (this->*t_converter)( t_in_raw[ t_index ], t_out_raw[ t_index ] );
                }

                out_stream< 0 >().state( stream::s_run );
                t_index = out_stream< 0 >()++;

                continue;
            }
            if( t_in_state == stream::s_stop )
            {
                out_stream< 0 >().state( stream::s_stop );
                t_index = out_stream< 0 >()++;

                continue;
            }
            if( t_in_state == stream::s_exit )
            {
                out_stream< 0 >().state( stream::s_exit );
                t_index = out_stream< 0 >()++;

                break;
            }
        }

        return;
    }

    void cf_rf_transformer::finalize()
    {
        out_buffer< 0 >().finalize();

        return;
    }

    void cf_rf_transformer::real( const complex_t& p_complex, real_t& p_real )
    {
        p_real = p_complex[ 0 ];
        return;
    }
    void cf_rf_transformer::imaginary( const complex_t& p_complex, real_t& p_real )
    {
        p_real = p_complex[ 1 ];
        return;
    }
    void cf_rf_transformer::modulus( const complex_t& p_complex, real_t& p_real )
    {
        p_real = sqrt( p_complex[ 0 ] * p_complex[ 0 ] + p_complex[ 1 ] * p_complex[ 1 ] );
        return;
    }
    void cf_rf_transformer::argument( const complex_t& p_complex, real_t& p_real )
    {
        p_real = atan2( p_complex[ 1 ], p_complex[ 0 ] );
        return;
    }

}
