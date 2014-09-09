#include "ct_ctf_fourier_transformer.hh"

#include "fourier.hh"

namespace midge
{

    ct_ctf_fourier_transformer::ct_ctf_fourier_transformer() :
            f_length( 10 )
    {
    }
    ct_ctf_fourier_transformer::~ct_ctf_fourier_transformer()
    {
    }

    void ct_ctf_fourier_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );
        return;
    }

    void ct_ctf_fourier_transformer::execute()
    {
        command_t t_in_command;

        ct_data* t_in_data;
        ctf_data* t_out_data;

        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;
        real_t t_frequency_interval;
        count_t t_frequency_index;

        fourier* t_fourier = fourier::get_instance();
        fourier_t* t_generator = NULL;

        while( true )
        {
            t_in_command = in_stream< 0 >().get();
            t_in_data = in_stream< 0 >().data();
            t_out_data = out_stream< 0 >().data();

            if( t_in_command == stream::s_start )
            {
                t_size = t_in_data->get_size();
                t_time_interval = t_in_data->get_time_interval();
                t_time_index = t_in_data->get_time_index();
                t_frequency_interval = 1. / (t_size * t_time_interval);
                t_frequency_index = 0;

                t_out_data->set_size( t_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index + t_size / 2 );
                t_out_data->set_frequency_interval( t_frequency_interval );
                t_out_data->set_frequency_index( t_frequency_index );

                t_generator = t_fourier->forward( t_size, &(t_in_data->at(0)), &(t_out_data->at(0)) );

                out_stream< 0 >().set( stream::s_start );
                continue;
            }
            if( t_in_command == stream::s_run )
            {
                t_time_interval = t_in_data->get_time_interval();
                t_time_index = t_in_data->get_time_index();
                t_frequency_interval = 1. / (t_size * t_time_interval);
                t_frequency_index = 0;

                t_out_data->set_size( t_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index + t_size / 2 );
                t_out_data->set_frequency_interval( t_frequency_interval );
                t_out_data->set_frequency_index( t_frequency_index );

                t_fourier->execute( t_generator, &(t_in_data->at(0)), &(t_out_data->at(0)) );

                out_stream< 0 >().set( stream::s_run );
                continue;
            }
            if( t_in_command == stream::s_stop )
            {
                t_fourier->destroy( t_generator );

                out_stream< 0 >().set( stream::s_stop );
                continue;
            }
            if( t_in_command == stream::s_exit )
            {
                out_stream< 0 >().set( stream::s_exit );
                return;
            }
        }

        return;
    }

    void ct_ctf_fourier_transformer::finalize()
    {
        out_buffer< 0 >().finalize();
        return;
    }

}
