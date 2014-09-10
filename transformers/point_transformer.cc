#include "point_transformer.hh"
#include "pool.hh"

#include <cmath>

namespace midge
{

    point_transformer::point_transformer() :
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
        return;
    }

    void point_transformer::execute()
    {
        index_t t_index;
        enum_t t_command;

        const rf_data* t_background;
        count_t t_background_size;
        real_t t_background_frequency_interval;
        count_t t_background_frequency_index;

        const rtf_data* t_signal;
        count_t t_signal_size;
        real_t t_signal_frequency_interval;
        count_t t_signal_frequency_index;
        real_t t_signal_time_interval;
        count_t t_signal_time_index;

        point_data* t_points;
        pointer< point > t_point;
        count_t t_begin_time_index;
        count_t t_end_time_index;
        count_t t_begin_frequency_index;
        count_t t_end_frequency_index;

        register real_t t_time_value;
        register real_t t_frequency_value;
        register real_t t_background_value;
        register real_t t_signal_value;
        register real_t t_ratio_value;

        while( true )
        {
            t_command = in_stream< 0 >().get();

            if( t_command == stream::s_start )
            {
                t_background = in_stream< 0 >().data();
                t_background_size = t_background->get_size();
                t_background_frequency_interval = t_background->get_frequency_interval();
                t_background_frequency_index = t_background->get_frequency_index();

                t_begin_frequency_index = (count_t) (round( f_begin_hz / t_background_frequency_interval ));
                if( t_background_frequency_index > t_begin_frequency_index )
                {
                    t_begin_frequency_index = t_background->get_frequency_index();
                }

                t_end_frequency_index = (count_t) (round( f_end_hz / t_background_frequency_interval ));
                if( t_background_frequency_index + t_background_frequency_interval * (t_background_size - 1) < t_end_frequency_index )
                {
                    t_end_frequency_index = t_background_frequency_index + t_background_frequency_interval * (t_background_size - 1);
                }

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

                if( t_background_frequency_interval != t_signal_frequency_interval )
                {
                    throw error() << "point transformer got background frequency interval of <" << t_background_frequency_interval << "> which does not match signal frequency interval of <" << t_signal_frequency_interval << ">";
                    return;
                }

                if( t_signal_frequency_index > t_begin_frequency_index )
                {
                    t_begin_frequency_index = t_signal->get_frequency_index();
                }

                if( t_signal_frequency_index + t_signal_frequency_interval * (t_signal_size - 1) < t_end_frequency_index )
                {
                    t_end_frequency_index = t_signal_frequency_index + t_signal_frequency_interval * (t_signal_size - 1);
                }

                t_begin_time_index = (count_t) (floor( f_begin_sec / t_signal_time_interval ));
                t_end_time_index = (count_t) (ceil( f_end_sec / t_signal_time_interval ));

                t_points->points().resize( t_end_frequency_index - t_begin_frequency_index + 1 );
                t_points->set_size( t_end_frequency_index - t_begin_frequency_index + 1 );
                t_points->set_time_interval( t_signal_time_interval );
                t_points->set_time_index( t_begin_time_index );
                t_points->set_frequency_interval( t_background_frequency_interval );
                t_points->set_frequency_index( t_begin_frequency_index );

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

                t_points->points().resize( t_end_frequency_index - t_begin_frequency_index + 1 );
                t_points->set_size( t_end_frequency_index - t_begin_frequency_index + 1 );
                t_points->set_time_interval( t_signal_time_interval );
                t_points->set_time_index( t_signal_time_index );
                t_points->set_frequency_interval( t_background_frequency_interval );
                t_points->set_frequency_index( t_begin_frequency_index );

                t_time_value = t_signal_time_interval * t_signal_time_index;
                pool< point >::allocate( t_points->points() );
                for( t_index = t_begin_frequency_index; t_index <= t_end_frequency_index; t_index++ )
                {
                    t_frequency_value = t_index * t_background_frequency_interval;
                    t_background_value = t_background->at( t_index - t_background_frequency_index );
                    t_signal_value = t_signal->at( t_index - t_signal_frequency_index );
                    t_ratio_value = t_signal_value / t_background_value;

                    t_point = t_points->points().at( t_index - t_begin_frequency_index );
                    t_point->time() = t_time_value;
                    t_point->frequency() = t_frequency_value;
                    t_point->ratio() = t_ratio_value;
                }

                out_stream< 0 >().set( stream::s_run );
                continue;
            }

            if( t_command == stream::s_stop )
            {
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
