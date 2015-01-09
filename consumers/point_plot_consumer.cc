#include "point_plot_consumer.hh"

#include "plot.hh"

#include <limits>
using std::numeric_limits;

namespace midge
{

    point_plot_consumer::point_plot_consumer() :
            f_plot_field( "id" ),
            f_plot_key( "" ),
            f_plot_name( "" ),
            f_chart_title( "" ),
            f_x_title( "" ),
            f_y_title( "" ),
            f_z_title( "" )
    {
    }
    point_plot_consumer::~point_plot_consumer()
    {
    }

    void point_plot_consumer::initialize()
    {
        plot::get_instance()->initialize();
        return;
    }

    void point_plot_consumer::execute()
    {
        count_t t_count;
        enum_t t_command;

        point_data* t_points;
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

        enum_t t_field;
        if( f_plot_field == "id" )
        {
            t_field = 0;
        }
        else if( f_plot_field == "ratio" )
        {
            t_field = 1;
        }
        else if( f_plot_field == "score" )
        {
            t_field = 2;
        }
        else
        {
            throw error() << "point plot consumer plot field must be either <id>, <ratio> or <score>";
            return;
        }

        t_count = 0;
        while( true )
        {
            t_command = in_stream< 0 >().get();
            t_points = in_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                continue;
            }
            if( t_command == stream::s_run )
            {
                t_size = t_points->size();
                t_frequency_interval = t_points->frequency_interval();
                t_frequency_index = t_points->frequency_index();
                t_time_interval = t_points->time_interval();
                t_time_index = t_points->time_index();

                for( vector< point >::iterator t_point_it = t_points->points().begin(); t_point_it != t_points->points().end(); t_point_it++ )
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

                return;
            }
        }

        return;
    }

    void point_plot_consumer::finalize()
    {
        plot::get_instance()->finalize();
        return;
    }

}
