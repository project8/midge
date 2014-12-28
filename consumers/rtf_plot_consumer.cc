#include "rtf_plot_consumer.hh"

#include "plot.hh"

#include <cmath>

namespace midge
{

    rtf_plot_consumer::rtf_plot_consumer() :
            f_plot_key( "" ),
            f_plot_name( "" ),
            f_chart_title( "" ),
            f_x_title( "" ),
            f_y_title( "" ),
            f_z_title( "" )
    {
    }
    rtf_plot_consumer::~rtf_plot_consumer()
    {
    }

    void rtf_plot_consumer::initialize()
    {
        plot::get_instance()->initialize();
        return;
    }

    void rtf_plot_consumer::execute()
    {
        index_t t_index;

        enum_t t_command;
        const rtf_data* t_data;
        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;
        real_t t_frequency_interval;
        count_t t_frequency_index;

        plot* t_plot = plot::get_instance();
        plot::abscissa t_x;
        plot::abscissa t_y;
        plot::ordinate t_z;

        count_t t_first_written_index;
        count_t t_last_written_index;
        count_t t_count = 0;

        while( true )
        {
            t_command = in_stream< 0 >().get();
            t_data = in_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                t_size = t_data->get_size();
                t_time_interval = t_data->get_time_interval();
                t_time_index = t_data->get_time_index();
                t_frequency_interval = t_data->get_frequency_interval();
                t_frequency_index = t_data->get_frequency_index();

                t_x.title() = f_x_title;
                t_y.title() = f_y_title;
                t_z.title() = f_z_title;

                t_first_written_index = t_time_index;
                t_last_written_index = t_time_index;

                continue;
            }
            if( t_command == stream::s_run )
            {
                t_time_index = t_data->get_time_index();

                for( t_index = t_frequency_index; t_index < t_frequency_index + t_size; t_index++ )
                {
                    t_x.values().push_back( t_time_index * t_time_interval );
                    t_y.values().push_back( t_index * t_frequency_interval );
                    t_z.values().push_back( t_data->at( t_index - t_frequency_index ) );
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

                t_plot->create_plot_two_dimensional( f_plot_key, f_plot_name, f_chart_title, t_x, t_y, t_z );

                continue;
            }
            if( t_command == stream::s_exit )
            {
                return;
            }
        }

        return;
    }

    void rtf_plot_consumer::finalize()
    {
        plot::get_instance()->finalize();
        return;
    }

}
