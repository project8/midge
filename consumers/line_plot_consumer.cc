#include "line_plot_consumer.hh"

#include "plot.hh"

#include <limits>
using std::numeric_limits;

namespace midge
{

    line_plot_consumer::line_plot_consumer() :
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
    line_plot_consumer::~line_plot_consumer()
    {
    }

    void line_plot_consumer::initialize()
    {
        plot::get_instance()->initialize();
        return;
    }

    void line_plot_consumer::execute()
    {
        count_t t_count;
        enum_t t_command;

        line_data* t_lines;
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
        vector< pair< pair< real_t, real_t >, pair< real_t, real_t > > > t_coordinates;

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
        else if( f_plot_field == "id" )
        {
            t_field = 3;
        }
        else if( f_plot_field == "time" )
        {
            t_field = 4;
        }
        else if( f_plot_field == "duration" )
        {
            t_field = 5;
        }
        else if( f_plot_field == "frequency" )
        {
            t_field = 6;
        }
        else if( f_plot_field == "slope" )
        {
            t_field = 7;
        }
        else if( f_plot_field == "correlation" )
        {
            t_field = 8;
        }
        else if( f_plot_field == "deviation" )
        {
            t_field = 9;
        }
        else if( f_plot_field == "score" )
        {
            t_field = 10;
        }
        else if( f_plot_field == "quality" )
        {
            t_field = 11;
        }
        else
        {
            throw error() << "line plot consumer plot field must be one of <point_id>, <point_ratio>, <point_score>, <id>, <time>, <duration>, <frequency>, <slope>, <correlation>, <deviation>, <score> or <quality>";
            return;
        }

        t_count = 0;
        while( true )
        {
            t_command = in_stream< 0 >().get();
            t_lines = in_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                continue;
            }
            if( t_command == stream::s_run )
            {
                t_size = t_lines->size();
                t_frequency_interval = t_lines->frequency_interval();
                t_frequency_index = t_lines->frequency_index();
                t_time_interval = t_lines->time_interval();
                t_time_index = t_lines->time_index();

                for( vector< line >::iterator t_line_it = t_lines->lines().begin(); t_line_it != t_lines->lines().end(); t_line_it++ )
                {
                    t_coordinates.resize( t_coordinates.size() + 1 );

                    for( line::point_cit t_point_it = t_line_it->points().begin(); t_point_it != t_line_it->points().end(); t_point_it++ )
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

                    t_coordinates.back().first.first = t_line_it->time();
                    t_coordinates.back().first.second = t_line_it->frequency();
                    t_coordinates.back().second.first = t_line_it->time() + t_line_it->duration();
                    t_coordinates.back().second.second = t_line_it->frequency() + t_line_it->slope() * t_line_it->duration();
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
                for( vector< line >::iterator t_line_it = t_lines->lines().begin(); t_line_it != t_lines->lines().end(); t_line_it++ )
                {
                    for( line::point_cit t_point_it = t_line_it->points().begin(); t_point_it != t_line_it->points().end(); t_point_it++ )
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

                    t_coordinates.resize( t_coordinates.size() + 1 );
                    t_coordinates.back().first.first = t_line_it->time();
                    t_coordinates.back().first.second = t_line_it->frequency();
                    t_coordinates.back().second.first = t_line_it->time() + t_line_it->duration();
                    t_coordinates.back().second.second = t_line_it->frequency() + t_line_it->slope() * t_line_it->duration();
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

                t_plot->create_plot_two_dimensional( f_plot_key, f_plot_name, f_chart_title, t_x, t_y, t_z );

                if( f_plot_overlay == true )
                {
                    for( vector< pair< pair< real_t, real_t >, pair< real_t, real_t > > >::iterator t_coordinates_it = t_coordinates.begin(); t_coordinates_it != t_coordinates.end(); t_coordinates_it++ )
                    {
                        t_x.values().clear();
                        t_x.values().push_back( t_coordinates_it->first.first );
                        t_x.values().push_back( t_coordinates_it->second.first );

                        t_y.values().clear();
                        t_y.values().push_back( t_coordinates_it->first.second );
                        t_y.values().push_back( t_coordinates_it->second.second );

                        t_plot->create_graph_two_dimensional( f_plot_key, f_plot_name, f_chart_title, t_x, t_y );
                    }
                }

                return;
            }
        }

        return;
    }

    void line_plot_consumer::finalize()
    {
        plot::get_instance()->finalize();
        return;
    }

}
