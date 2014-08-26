#include "rtf_rtf_ratio_transformer.hh"

#include "rf_data.hh"
#include "binary.hh"

namespace midge
{

    rtf_rtf_ratio_transformer::rtf_rtf_ratio_transformer() :
            f_file( "" ),
            f_length( 10 )
    {
    }
    rtf_rtf_ratio_transformer::~rtf_rtf_ratio_transformer()
    {
    }

    void rtf_rtf_ratio_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );

        return;
    }

    void rtf_rtf_ratio_transformer::execute()
    {
        count_t t_index;

        command_t t_in_command;

        rtf_data t_in_data;
        real_t* t_in_raw;

        rf_data t_background_data;
        real_t* t_background_raw;
        binary t_background_file;

        rtf_data t_out_data;
        real_t* t_out_raw;

        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;
        real_t t_frequency_interval;
        count_t t_frequency_index;

        t_background_file.read( f_file );
        t_background_file >> t_background_data;
        t_background_file >> t_background_data; //yes this is right, it has to pull twice. if you're wondering why that's the case, you can go fuck yourself.
        t_background_file.close();
        t_background_raw = t_background_data.raw();

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
                t_frequency_interval = t_in_data.get_frequency_interval();
                t_frequency_index = t_in_data.get_frequency_index();

                t_out_data.set_size( t_size );
                t_out_data.set_time_interval( t_time_interval );
                t_out_data.set_time_index( t_time_index );
                t_out_data.set_frequency_interval( t_frequency_interval );
                t_out_data.set_frequency_index( t_frequency_index );

                out_stream< 0 >().command( stream::s_start );
                out_stream< 0 >() << t_out_data;
                in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_run )
            {
                t_time_index = t_in_data.get_time_index();
                t_in_raw = t_in_data.raw();

                t_out_data.set_size( t_size );
                t_out_data.set_time_interval( t_time_interval );
                t_out_data.set_time_index( t_time_index );
                t_out_data.set_frequency_interval( t_frequency_interval );
                t_out_data.set_frequency_index( t_frequency_index );
                t_out_raw = t_out_data.raw();

                for( t_index = 0; t_index < t_size; t_index++ )
                {
                    t_out_raw[ t_index ] = (t_in_raw[ t_index ] - t_background_raw[ t_index ]) / t_background_raw[ t_index ];
                }

                out_stream< 0 >().command( stream::s_run );
                out_stream< 0 >() << t_out_data;
                in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_stop )
            {
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

    void rtf_rtf_ratio_transformer::finalize()
    {
        out_buffer< 0 >().finalize();

        return;
    }

}

