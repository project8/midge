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

        state_t t_in_state;
        const rt_data* t_in_data;
        const real_t* t_in_raw;
        count_t t_in_size;
        real_t t_in_time_interval;
        count_t t_in_first_index;
        count_t t_in_time_index;

        plot* t_plot = plot::get_instance();
        plot::abscissa t_x;
        plot::ordinate t_y;

        count_t t_first_unwritten_index;

        while( true )
        {
            in_stream< 0 >()++;t_in_state
            = in_stream< 0 >().state();
            t_in_data = in_stream< 0 >().data();

            if( t_in_state == stream::s_start )
            {
                t_in_size = t_in_data->get_size();
                t_in_time_interval = t_in_data->get_time_interval();
                t_in_first_index = t_in_data->get_time_index();

                t_x.title() = f_x_title;
                t_y.title() = f_y_title;

                t_first_unwritten_index = t_in_first_index;
                continue;
            }
            if( t_in_state == stream::s_run )
            {
                t_in_raw = t_in_data->raw();
                t_in_time_index = t_in_data->get_time_index();

                if( t_in_time_index < t_first_unwritten_index )
                {
                    for( t_index = t_first_unwritten_index; t_index < t_in_time_index + t_in_size; t_index++ )
                    {
                        t_x.values().push_back( t_index * t_in_time_interval );
                        t_y.values().push_back( t_in_raw[ t_index - t_in_time_index ] );
                    }
                }
                else
                {
                    for( t_index = t_first_unwritten_index; t_index < t_in_time_index; t_index++ )
                    {
                        t_x.values().push_back( t_index * t_in_time_interval );
                        t_y.values().push_back( 0. );
                    }
                    for( t_index = t_in_time_index; t_index < t_in_time_index + t_in_size; t_index++ )
                    {
                        t_x.values().push_back( t_index * t_in_time_interval );
                        t_y.values().push_back( t_in_raw[ t_index - t_in_time_index ] );
                    }
                }

                t_first_unwritten_index = t_in_time_index + t_in_size;

                continue;
            }
            if( t_in_state == stream::s_stop )
            {
                t_x.count() = t_first_unwritten_index - t_in_first_index;
                t_x.low() = t_in_first_index * t_in_time_interval;
                t_x.high() = (t_first_unwritten_index - 1) * t_in_time_interval;

                t_plot->plot_one_dimensional( f_plot_key, f_plot_name, f_chart_title, t_x, t_y );

                continue;
            }
            if( t_in_state == stream::s_exit )
            {
                break;
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
