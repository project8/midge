#include "cf_ct_fourier_transformer.hh"

#include "fourier.hh"

namespace midge
{

    cf_ct_fourier_transformer::cf_ct_fourier_transformer() :
            f_length( 10 )
    {
    }
    cf_ct_fourier_transformer::~cf_ct_fourier_transformer()
    {
    }

    void cf_ct_fourier_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );
        out_buffer< 0 >().set_name( get_name() );

        return;
    }

    void cf_ct_fourier_transformer::execute()
    {
        count_t t_index;

        state_t t_in_state;
        const cf_data* t_in_data;
        complex_t* t_in_raw;

        ct_data* t_out_data;
        complex_t* t_out_raw;

        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;

        fourier* t_fourier = fourier::get_instance();
        fourier_t* t_generator = NULL;

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

                t_in_raw = t_in_data->raw();
                t_out_raw = t_out_data->raw();

                t_generator = t_fourier->forward( t_size, t_in_raw, t_out_raw );

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
                t_out_data->set_time_index( t_time_index - t_size / 2 );
                t_out_raw = t_out_data->raw();

                t_fourier->execute( t_generator, t_in_raw, t_out_raw );

                out_stream< 0 >().state( stream::s_run );
                t_index = out_stream< 0 >()++;

                continue;
            }
            if( t_in_state == stream::s_stop )
            {
                t_fourier->destroy( t_generator );

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

    void cf_ct_fourier_transformer::finalize()
    {
        out_buffer< 0 >().finalize();

        return;
    }

}
