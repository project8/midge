#include "event_plot_consumer.hh"

#include "plot.hh"

#include <limits>
using std::numeric_limits;

namespace midge
{

    event_plot_consumer::event_plot_consumer() :
                f_plot_undermap( true ),
                f_plot_overlay( true ),
                f_plot_field( "id" ),
                f_plot_key( "" ),
                f_plot_name( "" ),
                f_chart_title( "" ),
                f_x_title( "" ),
                f_y_title( "" ),
                f_z_title( "" )
    {
    }
    event_plot_consumer::~event_plot_consumer()
    {
    }

    void event_plot_consumer::initialize()
    {
        plot::get_instance()->initialize();
        return;
    }

    void event_plot_consumer::execute()
    {
        count_t t_count;
        enum_t t_command;

        event_data* t_events;
        count_t t_size;
        real_t t_frequency_interval;
        count_t t_frequency_index;
        real_t t_time_interval;
        count_t t_time_index;
        count_t t_start_time_index = numeric_limits< count_t >::max();
        count_t t_stop_time_index = numeric_limits< count_t >::min();

        plot* t_plot = plot::get_instance();
        plot::abscissa t_x;
        plot::abscissa t_y;
        plot::ordinate t_z;
        vector< vector< pair< real_t, real_t > > > t_coordinates;

        enum_t t_field;
        if( f_plot_field == "point_id" )
        {
            t_field = 0;
        }
        else if( f_plot_field == "point_ratio" )
        {
            t_field = 1;
        }
        else if( f_plot_field == "point_score" )
        {
            t_field = 2;
        }
        else if( f_plot_field == "line_id" )
        {
            t_field = 3;
        }
        else if( f_plot_field == "line_frequency" )
        {
            t_field = 4;
        }
        else if( f_plot_field == "line_slope" )
        {
            t_field = 5;
        }
        else if( f_plot_field == "line_duration" )
        {
            t_field = 6;
        }
        else if( f_plot_field == "line_correlation" )
        {
            t_field = 7;
        }
        else if( f_plot_field == "line_deviation" )
        {
            t_field = 8;
        }
        else if( f_plot_field == "line_ratio" )
        {
            t_field = 9;
        }
        else if( f_plot_field == "line_score" )
        {
            t_field = 10;
        }
        else if( f_plot_field == "line_quality" )
        {
            t_field = 11;
        }
        else
        {
            throw error() << "event plot consumer plot field must be one of <point_id>, <point_ratio>, <point_score>, <line_id>, <line_frequency>, <line_slope>, <line_duration>, <line_correlation>, <line_deviation>, <line_ratio>, <line_score>, <line_quality>, <id>, <time>, <duration>, <frequency>, or <slope>";
            return;
        }

        t_count = 0;
        while( true )
        {
            t_command = in_stream< 0 >().get();
            t_events = in_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                continue;
            }
            if( t_command == stream::s_run )
            {
                t_size = t_events->size();
                t_frequency_interval = t_events->frequency_interval();
                t_frequency_index = t_events->frequency_index();
                t_time_interval = t_events->time_interval();
                t_time_index = t_events->time_index();

                for( vector< event >::const_iterator t_event_it = t_events->events().begin(); t_event_it != t_events->events().end(); t_event_it++ )
                {
                    t_coordinates.resize( t_coordinates.size() + 1 );

                    for( event::line_cit t_line_it = t_event_it->lines().begin(); t_line_it != t_event_it->lines().end(); ++t_line_it )
                    {
                        for( line::point_cit t_point_it = t_line_it->points().begin(); t_point_it != t_line_it->points().end(); ++t_point_it )
                        {
                            t_x.values().push_back( t_point_it->time() );
                            t_y.values().push_back( t_point_it->frequency() );
                            if( t_field == 0 )
                            {
                                t_z.values().push_back( t_point_it->id() );
                                continue;
                            }
                            if( t_field == 1 )
                            {
                                t_z.values().push_back( t_point_it->ratio() );
                                continue;
                            }
                            if( t_field == 2 )
                            {
                                t_z.values().push_back( t_point_it->score() );
                                continue;
                            }
                            if( t_field == 3 )
                            {
                                t_z.values().push_back( t_line_it->id() );
                                continue;
                            }
                            if( t_field == 4 )
                            {
                                t_z.values().push_back( t_line_it->time() );
                                continue;
                            }
                            if( t_field == 5 )
                            {
                                t_z.values().push_back( t_line_it->duration() );
                                continue;
                            }
                            if( t_field == 6 )
                            {
                                t_z.values().push_back( t_line_it->frequency() );
                                continue;
                            }
                            if( t_field == 7 )
                            {
                                t_z.values().push_back( t_line_it->slope() );
                                continue;
                            }
                            if( t_field == 8 )
                            {
                                t_z.values().push_back( t_line_it->correlation() );
                                continue;
                            }
                            if( t_field == 9 )
                            {
                                t_z.values().push_back( t_line_it->deviation() );
                                continue;
                            }
                            if( t_field == 10 )
                            {
                                t_z.values().push_back( t_line_it->score() );
                                continue;
                            }
                            if( t_field == 11 )
                            {
                                t_z.values().push_back( t_line_it->quality() );
                                continue;
                            }
                        }

                        t_coordinates.back().push_back( pair< real_t, real_t >( t_line_it->time(), t_line_it->frequency() ) );
                        t_coordinates.back().push_back( pair< real_t, real_t >( t_line_it->time() + t_line_it->duration(), t_line_it->frequency() + t_line_it->slope() * t_line_it->duration() ) );
                    }
                }

                if( t_start_time_index > t_time_index )
                {
                    t_start_time_index = t_time_index;
                }

                if( t_stop_time_index < t_time_index )
                {
                    t_stop_time_index = t_time_index;
                }

                t_count++;

                continue;
            }
            if( t_command == stream::s_stop )
            {
                for( vector< event >::const_iterator t_event_it = t_events->events().begin(); t_event_it != t_events->events().end(); t_event_it++ )
                {
                    t_coordinates.resize( t_coordinates.size() + 1 );

                    for( event::line_cit t_line_it = t_event_it->lines().begin(); t_line_it != t_event_it->lines().end(); ++t_line_it )
                    {
                        for( line::point_cit t_point_it = t_line_it->points().begin(); t_point_it != t_line_it->points().end(); ++t_point_it )
                        {
                            t_x.values().push_back( t_point_it->time() );
                            t_y.values().push_back( t_point_it->frequency() );
                            if( t_field == 0 )
                            {
                                t_z.values().push_back( t_point_it->id() );
                                continue;
                            }
                            if( t_field == 1 )
                            {
                                t_z.values().push_back( t_point_it->ratio() );
                                continue;
                            }
                            if( t_field == 2 )
                            {
                                t_z.values().push_back( t_point_it->score() );
                                continue;
                            }
                            if( t_field == 3 )
                            {
                                t_z.values().push_back( t_line_it->id() );
                                continue;
                            }
                            if( t_field == 4 )
                            {
                                t_z.values().push_back( t_line_it->time() );
                                continue;
                            }
                            if( t_field == 5 )
                            {
                                t_z.values().push_back( t_line_it->duration() );
                                continue;
                            }
                            if( t_field == 6 )
                            {
                                t_z.values().push_back( t_line_it->frequency() );
                                continue;
                            }
                            if( t_field == 7 )
                            {
                                t_z.values().push_back( t_line_it->slope() );
                                continue;
                            }
                            if( t_field == 8 )
                            {
                                t_z.values().push_back( t_line_it->correlation() );
                                continue;
                            }
                            if( t_field == 9 )
                            {
                                t_z.values().push_back( t_line_it->deviation() );
                                continue;
                            }
                            if( t_field == 10 )
                            {
                                t_z.values().push_back( t_line_it->score() );
                                continue;
                            }
                            if( t_field == 11 )
                            {
                                t_z.values().push_back( t_line_it->quality() );
                                continue;
                            }
                        }

                        t_coordinates.back().push_back( pair< real_t, real_t >( t_line_it->time(), t_line_it->frequency() ) );
                        t_coordinates.back().push_back( pair< real_t, real_t >( t_line_it->time() + t_line_it->duration(), t_line_it->frequency() + t_line_it->slope() * t_line_it->duration() ) );
                    }
                }

                continue;
            }
            if( t_command == stream::s_exit )
            {
                t_x.count() = t_count;
                t_x.low() = t_start_time_index * t_time_interval;
                t_x.high() = t_stop_time_index * t_time_interval;

                t_y.count() = t_size;
                t_y.low() = t_frequency_index * t_frequency_interval;
                t_y.high() = (t_frequency_index + t_size - 1) * t_frequency_interval;

                t_x.title() = f_x_title;
                t_y.title() = f_y_title;
                t_z.title() = f_z_title;

                if( f_plot_undermap == true )
                {
                    t_plot->create_plot_two_dimensional( f_plot_key, f_plot_name, f_chart_title, t_x, t_y, t_z );
                }

                if( f_plot_overlay == true )
                {
                    for( vector< vector< pair< real_t, real_t > > >::iterator t_coordinates_it = t_coordinates.begin(); t_coordinates_it != t_coordinates.end(); t_coordinates_it++ )
                    {
                        t_x.values().clear();
                        t_y.values().clear();

                        for( vector< pair< real_t, real_t > >::iterator t_coordinate_it = t_coordinates_it->begin(); t_coordinate_it != t_coordinates_it->end(); t_coordinate_it++ )
                        {
                            t_x.values().push_back( t_coordinate_it->first );
                            t_y.values().push_back( t_coordinate_it->second );
                        }

                        t_plot->create_graph_two_dimensional( f_plot_key, f_plot_name, f_chart_title, t_x, t_y );
                    }
                }

                return;
            }
        }

        return;
    }

    void event_plot_consumer::finalize()
    {
        plot::get_instance()->finalize();
        return;
    }

}
