#include "rt_ct_analytic_transformer.hh"

#include "fourier.hh"

#include <cmath>

namespace midge
{

    rt_ct_analytic_transformer::rt_ct_analytic_transformer() :
            f_length( 10 )
    {
    }
    rt_ct_analytic_transformer::~rt_ct_analytic_transformer()
    {
    }

    void rt_ct_analytic_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );
        return;
    }

    void rt_ct_analytic_transformer::execute()
    {
        index_t t_index;

        enum_t t_in_command;

        rt_data* t_in_data;
        ct_data* t_out_data;

        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;

        count_t t_under;
        count_t t_nyquist;
        count_t t_over;
        real_t t_norm;

        fourier* t_fourier = fourier::get_instance();
        complex_t* t_signal = NULL;
        complex_t* t_transform = NULL;
        complex_t* t_analytic = NULL;
        fourier_t* t_forward_generator = NULL;
        fourier_t* t_backward_generator = NULL;

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
                t_norm = 1. / (real_t) ( t_size );

                t_time_interval = t_in_data->get_time_interval();
                t_time_index = t_in_data->get_time_index();

                t_signal = t_fourier->allocate< complex_t >( t_size );
                t_transform = t_fourier->allocate< complex_t >( t_size );
                t_analytic = t_fourier->allocate< complex_t >( t_size );
                t_forward_generator = t_fourier->forward( t_size, t_signal, t_transform );
                t_backward_generator = t_fourier->backward( t_size, t_transform, t_analytic );

                t_out_data->set_size( t_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index );

                out_stream< 0 >().set( stream::s_start );
                continue;
            }
            if( t_in_command == stream::s_run )
            {
                t_time_interval = t_in_data->get_time_interval();
                t_time_index = t_in_data->get_time_index();

                t_out_data->set_size( t_size );
                t_out_data->set_time_interval( t_time_interval );
                t_out_data->set_time_index( t_time_index );

                // input to complex signal
                for( t_index = 0; t_index < t_size; t_index++ )
                {
                    t_signal[ t_index ][ 0 ] = t_in_data->at( t_index );
                    t_signal[ t_index ][ 1 ] = 0.;
                }

                // complex signal to complex spectrum
                t_fourier->execute( t_forward_generator );

                // complex spectrum under hilbert transformation
                t_transform[ 0 ][ 0 ] = 1. * t_transform[ 0 ][ 0 ] * t_norm;
                t_transform[ 0 ][ 1 ] = 1. * t_transform[ 0 ][ 1 ] * t_norm;
                for( t_index = 1; t_index <= t_under; t_index++ )
                {
                    t_transform[ t_index ][ 0 ] = 2. * t_transform[ t_index ][ 0 ] * t_norm;
                    t_transform[ t_index ][ 1 ] = 2. * t_transform[ t_index ][ 1 ] * t_norm;
                }
                if( t_nyquist != 0 )
                {
                    t_transform[ t_nyquist ][ 0 ] = 1. * t_transform[ t_nyquist ][ 0 ] * t_norm;
                    t_transform[ t_nyquist ][ 1 ] = 1. * t_transform[ t_nyquist ][ 1 ] * t_norm;
                }
                for( t_index = t_over; t_index < t_size; t_index++ )
                {
                    t_transform[ t_index ][ 0 ] = 0. * t_norm;
                    t_transform[ t_index ][ 1 ] = 0. * t_norm;
                }

                // complex spectrum to analytic signal
                t_fourier->execute( t_backward_generator );

                // analytic signal to output
                for( t_index = 0; t_index < t_size; t_index++ )
                {
                    t_out_data->at( t_index )[ 0 ] = t_analytic[ t_index ][ 0 ];
                    t_out_data->at( t_index )[ 1 ] = t_analytic[ t_index ][ 1 ];
                }

                out_stream< 0 >().set( stream::s_run );
                continue;
            }
            if( t_in_command == stream::s_stop )
            {
                t_fourier->free< complex_t >( t_signal );
                t_fourier->free< complex_t >( t_transform );
                t_fourier->free< complex_t >( t_analytic );
                t_fourier->destroy( t_forward_generator );
                t_fourier->destroy( t_backward_generator );

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

    void rt_ct_analytic_transformer::finalize()
    {
        out_buffer< 0 >().finalize();
        return;
    }

}
