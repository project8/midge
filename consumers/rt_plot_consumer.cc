#include "rt_plot_consumer.hh"

#include "plot.hh"

#include <cmath>

namespace midge
{

    rt_plot_consumer::rt_plot_consumer() :
            f_plot_key( "" ),
            f_plot_name( "" ),
            f_chart_title( "" ),
            f_x_title( "" ),
            f_y_title( "" )
    {
    }
    rt_plot_consumer::~rt_plot_consumer()
    {
    }

    void rt_plot_consumer::initialize()
    {
        plot::get_instance()->initialize();
        return;
    }

    void rt_plot_consumer::execute()
    {
        count_t t_index;

        command_t t_command;
        rt_data t_data;
        real_t* t_raw;
        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;

        plot* t_plot = plot::get_instance();
        plot::abscissa t_x;
        plot::ordinate t_y;

        count_t t_first_written_index;
        count_t t_first_unwritten_index;

        while( true )
        {
            in_stream< 0 >() >> t_data;
            t_command = in_stream< 0 >().command();

            if( t_command == stream::s_start )
            {
                t_size = t_data.get_size();
                t_time_interval = t_data.get_time_interval();
                t_time_index = t_data.get_time_index();

                t_x.title() = f_x_title;
                t_y.title() = f_y_title;

                t_first_written_index = t_time_index;
                t_first_unwritten_index = t_time_index;

                continue;
            }
            if( t_command == stream::s_run )
            {
                t_raw = t_data.raw();
                t_time_index = t_data.get_time_index();

                if( t_time_index < t_first_unwritten_index )
                {
                    for( t_index = t_first_unwritten_index; t_index < t_time_index + t_size; t_index++ )
                    {
                        t_x.values().push_back( t_index * t_time_interval );
                        t_y.values().push_back( t_raw[ t_index - t_time_index ] );
                    }
                }
                else
                {
                    for( t_index = t_first_unwritten_index; t_index < t_time_index; t_index++ )
                    {
                        t_x.values().push_back( t_index * t_time_interval );
                        t_y.values().push_back( 0. );
                    }
                    for( t_index = t_time_index; t_index < t_time_index + t_size; t_index++ )
                    {
                        t_x.values().push_back( t_index * t_time_interval );
                        t_y.values().push_back( t_raw[ t_index - t_time_index ] );
                    }
                }

                t_first_unwritten_index = t_time_index + t_size;

                continue;
            }
            if( t_command == stream::s_stop )
            {
                t_x.count() = t_first_unwritten_index - t_first_written_index;
                t_x.low() = t_first_written_index * t_time_interval;
                t_x.high() = (t_first_unwritten_index - 1) * t_time_interval;

                t_plot->plot_one_dimensional( f_plot_key, f_plot_name, f_chart_title, t_x, t_y );

                continue;
            }
            if( t_command == stream::s_exit )
            {
                return;
            }
        }

        return;
    }

    void rt_plot_consumer::finalize()
    {
        plot::get_instance()->finalize();
        return;
    }

}
