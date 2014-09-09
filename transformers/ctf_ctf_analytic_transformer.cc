#include "ctf_ctf_analytic_transformer.hh"

#include <cmath>

namespace midge
{

    ctf_ctf_analytic_transformer::ctf_ctf_analytic_transformer() :
            f_length( 10 )
    {
    }
    ctf_ctf_analytic_transformer::~ctf_ctf_analytic_transformer()
    {
    }

    void ctf_ctf_analytic_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );
        return;
    }

    void ctf_ctf_analytic_transformer::execute()
    {
        count_t t_index;

        command_t t_in_command;

        ctf_data* t_in_data;
        ctf_data* t_out_data;

        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;
        real_t t_frequency_interval;
        count_t t_frequency_index;

        count_t t_under;
        count_t t_nyquist;
        count_t t_over;
        real_t t_norm;

        while( true )
        {
            t_in_command = in_stream< 0 >().get();
            t_in_data = in_stream< 0 >().data();
            t_out_data = out_stream< 0 >().data();

            if( t_in_command == stream::s_start )
            {
                t_size = t_in_data->get_size();
                if( t_size % 2 == 0 )
                {
                    t_under = (t_size / 2) - 1;
                    t_nyquist = (t_size / 2);
                    t_over = (t_size / 2) + 1;
                }
                else
                {
                    t_under = (t_size - 1) / 2;
                    t_nyquist = 0;
                    t_over = (t_size + 1) / 2;
                }
                t_norm = 1. / (real_t) (t_size);

                t_time_interval = t_in_data->get_time_interval();
                t_time_index = t_in_data->get_time_index();
                t_frequency_interval = t_in_data->get_frequency_interval();
                t_frequency_index = t_in_data->get_frequency_index();

                t_out_data->set_size( t_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index );
                t_out_data->set_frequency_interval( t_frequency_interval );
                t_out_data->set_frequency_index( t_frequency_index );

                out_stream< 0 >().set( stream::s_start );
                continue;
            }
            if( t_in_command == stream::s_run )
            {
                t_time_interval = t_in_data->get_time_interval();
                t_time_index = t_in_data->get_time_index();
                t_frequency_interval = t_in_data->get_frequency_interval();
                t_frequency_index = t_in_data->get_frequency_index();

                t_out_data->set_size( t_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index );
                t_out_data->set_frequency_interval( t_frequency_interval );
                t_out_data->set_frequency_index( t_frequency_index );

                // complex spectrum under hilbert transformation
                t_out_data->at( 0 )[ 0 ] = 1. * t_in_data->at( 0 )[ 0 ] * t_norm;
                t_out_data->at( 0 )[ 1 ] = 1. * t_in_data->at( 0 )[ 1 ] * t_norm;
                for( t_index = 1; t_index <= t_under; t_index++ )
                {
                    t_out_data->at( t_index )[ 0 ] = 2. * t_in_data->at( t_index )[ 0 ] * t_norm;
                    t_out_data->at( t_index )[ 1 ] = 2. * t_in_data->at( t_index )[ 1 ] * t_norm;
                }
                if( t_nyquist != 0 )
                {
                    t_out_data->at( t_nyquist )[ 0 ] = 1. * t_in_data->at( t_nyquist )[ 0 ] * t_norm;
                    t_out_data->at( t_nyquist )[ 1 ] = 1. * t_in_data->at( t_nyquist )[ 1 ] * t_norm;
                }
                for( t_index = t_over; t_index < t_size; t_index++ )
                {
                    t_out_data->at( t_index )[ 0 ] = 0. * t_norm;
                    t_out_data->at( t_index )[ 1 ] = 0. * t_norm;
                }

                out_stream< 0 >().set( stream::s_run );
                continue;
            }
            if( t_in_command == stream::s_stop )
            {
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

    void ctf_ctf_analytic_transformer::finalize()
    {
        out_buffer< 0 >().finalize();
        return;
    }

}
