#include "point_transformer.hh"

#include <cmath>

namespace midge
{

    point_transformer::point_transformer() :
                f_threshold( 2. ),
                f_high_factor( 2. ),
                f_high_power( 1. ),
                f_low_factor( 1. ),
                f_low_power( 2. ),
                f_begin_sec( 0. ),
                f_end_sec( 10. ),
                f_begin_hz( 5. ),
                f_end_hz( 120.e6 ),
                f_length( 10 )
    {
    }

    point_transformer::~point_transformer()
    {
    }

    void point_transformer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );

        point::set_threshold( f_threshold );
        point::set_high_factor( f_high_factor );
        point::set_high_power( f_high_power );
        point::set_low_factor( f_low_factor );
        point::set_low_power( f_low_power );

        return;
    }

    void point_transformer::execute()
    {
        index_t t_index;
        enum_t t_command;

        rf_data* t_background;
        count_t t_background_size;
        real_t t_background_frequency_interval;
        count_t t_background_frequency_index;

        rtf_data* t_signal;
        count_t t_signal_size;
        real_t t_signal_frequency_interval;
        count_t t_signal_frequency_index;
        real_t t_signal_time_interval;
        count_t t_signal_time_index;

        point_data* t_points;
        count_t t_begin_time_index;
        count_t t_end_time_index;
        count_t t_begin_frequency_index;
        count_t t_end_frequency_index;

        count_t t_temp = 0;

        while( true )
        {
            t_command = in_stream< 0 >().get();

            if( t_command == stream::s_start )
            {
                t_background = in_stream< 0 >().data();
                t_background_size = t_background->get_size();
                t_background_frequency_interval = t_background->get_frequency_interval();
                t_background_frequency_index = t_background->get_frequency_index();

                continue;
            }

            if( t_command == stream::s_run )
            {
                t_background = in_stream< 0 >().data();

                continue;
            }

            if( t_command == stream::s_stop )
            {
                continue;
            }

            if( t_command == stream::s_exit )
            {
                break;
            }
        }

        while( true )
        {
            t_command = in_stream< 1 >().get();
            t_signal = in_stream< 1 >().data();
            t_points = out_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                t_signal_size = t_signal->get_size();
                t_signal_frequency_interval = t_signal->get_frequency_interval();
                t_signal_frequency_index = t_signal->get_frequency_index();
                t_signal_time_interval = t_signal->get_time_interval();
                t_signal_time_index = t_signal->get_time_index();

                if( t_background_frequency_interval != t_signal_frequency_interval )
                {
                    throw error() << "point transformer got background frequency interval of <" << t_background_frequency_interval << "> which does not match signal frequency interval of <" << t_signal_frequency_interval << ">";
                    return;
                }
                if( t_background_frequency_index != t_signal_frequency_index )
                {
                    throw error() << "point transformer got background frequency index of <" << t_background_frequency_index << "> which does not match signal frequency index of <" << t_signal_frequency_index << ">";
                    return;
                }

                t_begin_frequency_index = (count_t) (round( f_begin_hz / t_signal_frequency_interval ));
                if( t_signal_frequency_index > t_begin_frequency_index )
                {
                    t_begin_frequency_index = t_signal_frequency_index;
                }

                t_end_frequency_index = (count_t) (round( f_end_hz / t_signal_frequency_interval ));
                if( t_signal_frequency_index + t_signal_frequency_interval * (t_signal_size - 1) < t_end_frequency_index )
                {
                    t_end_frequency_index = t_signal_frequency_index + t_signal_frequency_interval * (t_signal_size - 1);
                }

                t_begin_time_index = (count_t) (round( f_begin_sec / t_signal_time_interval ));
                t_end_time_index = (count_t) (round( f_end_sec / t_signal_time_interval ));

                t_points->size() = t_end_frequency_index - t_begin_frequency_index + 1;
                t_points->frequency_interval() = t_signal_frequency_interval;
                t_points->frequency_index() = t_begin_frequency_index;
                t_points->time_interval() = t_signal_time_interval;
                t_points->time_index() = t_begin_time_index;

                t_points->points().clear();

                out_stream< 0 >().set( stream::s_start );
                continue;
            }

            if( t_command == stream::s_run )
            {
                t_signal_time_index = t_signal->get_time_index();
                if( t_signal_time_index < t_begin_time_index )
                {
                    continue;
                }
                if( t_signal_time_index > t_end_time_index )
                {
                    continue;
                }

                t_points->size() = t_end_frequency_index - t_begin_frequency_index + 1;
                t_points->frequency_interval() = t_signal_frequency_interval;
                t_points->frequency_index() = t_begin_frequency_index;
                t_points->time_interval() = t_signal_time_interval;
                t_points->time_index() = t_signal_time_index;

                t_points->points().resize( t_points->size() );
                point::set_background_data( t_background );
                point::set_signal_data( t_signal );

                for( t_index = t_begin_frequency_index; t_index <= t_end_frequency_index; t_index++ )
                {
                    t_points->points().at( t_index - t_begin_frequency_index ).initialize( t_index - t_signal_frequency_index );
                }

                t_temp++;

                out_stream< 0 >().set( stream::s_run );
                continue;
            }

            if( t_command == stream::s_stop )
            {
                t_points->points().clear();
                t_points->size() = t_end_frequency_index - t_begin_frequency_index + 1;
                t_points->frequency_interval() = t_background_frequency_interval;
                t_points->frequency_index() = t_begin_frequency_index;
                t_points->time_interval() = t_signal_time_interval;
                t_points->time_index() = t_end_time_index;

                out_stream< 0 >().set( stream::s_stop );
                continue;
            }

            if( t_command == stream::s_exit )
            {
                out_stream< 0 >().set( stream::s_exit );
                return;
            }
        }
    }

    void point_transformer::finalize()
    {
        out_buffer< 0 >().finalize();
        return;
    }

}
