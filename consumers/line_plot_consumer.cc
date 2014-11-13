#include "line_plot_consumer.hh"

#include "plot.hh"

#include <cmath>

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
        index_t t_index;

        enum_t t_command;
        const line_data* t_lines;
        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;
        real_t t_frequency_interval;
        count_t t_frequency_index;

        plot* t_plot = plot::get_instance();
        plot::abscissa t_x;
        plot::abscissa t_y;
        plot::ordinate t_z;
        vector< pair< pair< real_t, real_t >, pair< real_t, real_t > > > t_coordinates;

        count_t t_first_written_index;
        count_t t_last_written_index;
        count_t t_count = 0;

        enum_t t_field;
        if( f_plot_field == "ratio" )
        {
            t_field = 0;
        }
        else if( f_plot_field == "id" )
        {
            t_field = 1;
        }
        else if( f_plot_field == "frequency" )
        {
            t_field = 2;
        }
        else if( f_plot_field == "slope" )
        {
            t_field = 3;
        }
        else if( f_plot_field == "duration" )
        {
            t_field = 4;
        }
        else if( f_plot_field == "correlation" )
        {
            t_field = 5;
        }
        else if( f_plot_field == "deviation" )
        {
            t_field = 6;
        }
        else if( f_plot_field == "quality" )
        {
            t_field = 7;
        }
        else
        {
            throw error() << "line plot consumer plot field must be one of <ratio>, <id>, <frequency>, <slope>, <duration>, <correlation> or <deviation>";
            return;
        }

        while( true )
        {
            t_command = in_stream< 0 >().get();
            t_lines = in_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                //msg_warning( coremsg, "line plot consumer starting:" << eom );

                t_size = t_lines->get_size();
                t_time_interval = t_lines->get_time_interval();
                t_time_index = t_lines->get_time_index();
                t_frequency_interval = t_lines->get_frequency_interval();
                t_frequency_index = t_lines->get_frequency_index();

                t_x.title() = f_x_title;
                t_y.title() = f_y_title;
                t_z.title() = f_z_title;

                t_first_written_index = t_time_index;
                t_last_written_index = t_time_index;

                //msg_warning( coremsg, "  size is <" << t_size << ">" << eom );
                //msg_warning( coremsg, "  time interval is <" << t_time_interval << ">" << eom );
                //msg_warning( coremsg, "  time index is <" << t_time_index << ">" << eom );
                //msg_warning( coremsg, "  frequency interval is <" << t_frequency_interval << ">" << eom );
                //msg_warning( coremsg, "  frequency index is <" << t_frequency_index << ">" << eom );

                continue;
            }
            if( t_command == stream::s_run )
            {
                //msg_warning( coremsg, "line plot consumer executing:" << eom );

                t_time_index = t_lines->get_time_index();

                //msg_warning( coremsg, "  time index is <" << t_time_index << ">" << eom );

                for( t_index = 0; t_index < t_lines->lines().size(); t_index++ )
                {
                    for( line::point_cit t_it = t_lines->lines().at( t_index ).points().begin(); t_it != t_lines->lines().at( t_index ).points().end(); ++t_it )
                    {
                        t_x.values().push_back( t_it->time() );
                        t_y.values().push_back( t_it->frequency() );
                        if( t_field == 0 )
                        {
                            t_z.values().push_back( t_it->ratio() );
                            continue;
                        }
                        if( t_field == 1 )
                        {
                            t_z.values().push_back( t_lines->lines().at( t_index ).id() );
                            continue;
                        }
                        if( t_field == 2 )
                        {
                            t_z.values().push_back( t_lines->lines().at( t_index ).frequency() );
                            continue;
                        }
                        if( t_field == 3 )
                        {
                            t_z.values().push_back( t_lines->lines().at( t_index ).slope() );
                            continue;
                        }
                        if( t_field == 4 )
                        {
                            t_z.values().push_back( t_lines->lines().at( t_index ).duration() );
                            continue;
                        }
                        if( t_field == 5 )
                        {
                            t_z.values().push_back( t_lines->lines().at( t_index ).correlation() );
                            continue;
                        }
                        if( t_field == 6 )
                        {
                            t_z.values().push_back( t_lines->lines().at( t_index ).deviation() );
                            continue;
                        }
                        if( t_field == 7 )
                        {
                            t_z.values().push_back( t_lines->lines().at( t_index ).quality() );
                            continue;
                        }
                    }

                    t_coordinates.resize( t_coordinates.size() + 1 );
                    t_coordinates.back().first.first = t_lines->lines().at( t_index ).time();
                    t_coordinates.back().first.second = t_lines->lines().at( t_index ).frequency();
                    t_coordinates.back().second.first = t_lines->lines().at( t_index ).time() + t_lines->lines().at( t_index ).duration();
                    t_coordinates.back().second.second = t_lines->lines().at( t_index ).frequency() + t_lines->lines().at( t_index ).slope() * t_lines->lines().at( t_index ).duration();
                }

                t_last_written_index = t_time_index;
                t_count++;

                continue;
            }
            if( t_command == stream::s_stop )
            {
                t_x.count() = t_count;
                t_x.low() = t_first_written_index * t_time_interval;
                t_x.high() = t_last_written_index * t_time_interval;

                t_y.count() = t_size;
                t_y.low() = t_frequency_index * t_frequency_interval;
                t_y.high() = (t_frequency_index + t_size - 1) * t_frequency_interval;

                t_plot->plot_two_dimensional( f_plot_key, f_plot_name, f_chart_title, t_x, t_y, t_z );

                if( f_plot_overlay == true )
                {
                    for( t_index = 0; t_index < t_coordinates.size(); t_index++ )
                    {
                        t_x.values().clear();
                        t_x.values().push_back( t_coordinates.at( t_index ).first.first );
                        t_x.values().push_back( t_coordinates.at( t_index ).second.first );

                        t_y.values().clear();
                        t_y.values().push_back( t_coordinates.at( t_index ).first.second );
                        t_y.values().push_back( t_coordinates.at( t_index ).second.second );

                        t_plot->graph_two_dimensional( f_plot_key, f_plot_name, f_chart_title, t_x, t_y );
                    }
                }

                continue;
            }
            if( t_command == stream::s_exit )
            {
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
