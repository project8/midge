#include "rt_ct_transformer.hh"

#include <cmath>

namespace midge
{

    rt_ct_transformer::rt_ct_transformer() :
            f_mode( "real" ),
            f_length( 10 )
    {
    }
    rt_ct_transformer::~rt_ct_transformer()
    {
    }

    void rt_ct_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );
        out_buffer< 0 >().set_name( get_name() );

        return;
    }

    void rt_ct_transformer::execute()
    {
        count_t t_index;

        state_t t_in_state;
        const rt_data* t_in_data;
        const real_t* t_in_raw;

        ct_data* t_out_data;
        complex_t* t_out_raw;

        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;

        void (rt_ct_transformer::*t_converter)( const real_t&, complex_t& );

        if( f_mode == string( "real" ) )
        {
            t_converter = &rt_ct_transformer::real;
        }
        else if( f_mode == string( "imaginary" ) )
        {
            t_converter = &rt_ct_transformer::imaginary;
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

                t_out_data->set_size( t_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index );

                out_stream< 0 >().state( stream::s_start );
                t_index = out_stream< 0 >()++;

                continue;
            }
            if( t_in_state == stream::s_run )
            {
                t_time_index = t_in_data->get_time_index();
                t_in_raw = t_in_data->raw();

                t_out_data->set_size( t_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index );
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

    void rt_ct_transformer::finalize()
    {
        out_buffer< 0 >().finalize();

        return;
    }

    void rt_ct_transformer::real( const real_t& p_real, complex_t& p_complex )
    {
        p_complex[ 0 ] = p_real;
        p_complex[ 1 ] = 0.;
        return;
    }
    void rt_ct_transformer::imaginary( const real_t& p_real, complex_t& p_complex )
    {
        p_complex[ 0 ] = 0.;
        p_complex[ 1 ] = p_real;
        return;
    }

}
