#include "ctf_ct_fourier_transformer.hh"

#include "fourier.hh"

namespace midge
{

    ctf_ct_fourier_transformer::ctf_ct_fourier_transformer() :
            f_length( 10 )
    {
    }
    ctf_ct_fourier_transformer::~ctf_ct_fourier_transformer()
    {
    }

    void ctf_ct_fourier_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );
        return;
    }

    void ctf_ct_fourier_transformer::execute()
    {
        command_t t_in_command;

        ctf_data t_in_data;
        complex_t* t_in_raw;

        ct_data t_out_data;
        complex_t* t_out_raw;

        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;

        fourier* t_fourier = fourier::get_instance();
        fourier_t* t_generator = NULL;

        while( true )
        {
            in_stream< 0 >() >> t_in_data;
            out_stream< 0 >() >> t_out_data;
            t_in_command = in_stream< 0 >().command();

            if( t_in_command == stream::s_start )
            {
                t_size = t_in_data.get_size();
                t_time_interval = t_in_data.get_time_interval();
                t_time_index = t_in_data.get_time_index();

                t_out_data.set_size( t_size );
                t_out_data.set_time_interval( t_time_interval );
                t_out_data.set_time_index( t_time_index - t_size / 2 );

                t_in_raw = t_in_data.raw();
                t_out_raw = t_out_data.raw();

                t_generator = t_fourier->backward( t_size, t_in_raw, t_out_raw );

                out_stream< 0 >().command( stream::s_start );
                out_stream< 0 >() << t_out_data;
                in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_run )
            {
                t_time_interval = t_in_data.get_time_interval();
                t_time_index = t_in_data.get_time_index();
                t_in_raw = t_in_data.raw();

                t_out_data.set_size( t_size );
                t_out_data.set_time_interval( t_time_interval );
                t_out_data.set_time_index( t_time_index - t_size / 2 );
                t_out_raw = t_out_data.raw();

                t_fourier->execute( t_generator, t_in_raw, t_out_raw );

                out_stream< 0 >().command( stream::s_run );
                out_stream< 0 >() << t_out_data;
                in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_stop )
            {
                t_fourier->destroy( t_generator );

                out_stream< 0 >().command( stream::s_stop );
                out_stream< 0 >() << t_out_data;
                in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_exit )
            {
                out_stream< 0 >().command( stream::s_exit );
                out_stream< 0 >() << t_out_data;
                in_stream< 0 >() << t_in_data;
                return;
            }
        }

        return;
    }

    void ctf_ct_fourier_transformer::finalize()
    {
        out_buffer< 0 >().finalize();
        return;
    }

}