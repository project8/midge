#include "rf_ascii_consumer.hh"

namespace midge
{

    rf_ascii_consumer::rf_ascii_consumer() :
            f_file( "" )
    {
    }
    rf_ascii_consumer::~rf_ascii_consumer()
    {
    }

    void rf_ascii_consumer::initialize()
    {
        return;
    }

    void rf_ascii_consumer::execute()
    {
        count_t t_index;

        state_t t_in_state;
        const rf_data* t_in_data;
        const real_t* t_in_raw;

        count_t t_size;
        real_t t_time_interval;
        count_t t_time_index;
        real_t t_frequency_interval;
        count_t t_frequency_index;

        ofstream t_stream;

        while( true )
        {
            in_stream< 0 >()++;
            t_in_state = in_stream< 0 >().state();
            t_in_data = in_stream< 0 >().data();

            if( t_in_state == stream::s_start )
            {
                t_stream.open( f_file, std::ios_base::trunc );
                continue;
            }
            if( t_in_state == stream::s_run )
            {
                t_in_raw = t_in_data->raw();
                t_size = t_in_data->get_size();
                t_time_interval = t_in_data->get_time_interval();
                t_time_index = t_in_data->get_time_index();
                t_frequency_interval = t_in_data->get_frequency_interval();
                t_frequency_index = t_in_data->get_frequency_index();

                for( t_index = t_frequency_index; t_index < t_frequency_index + t_size; t_index++ )
                {
                    t_stream << t_time_interval * t_time_index << " " << t_frequency_interval * t_index << " " << t_in_raw[ t_index - t_frequency_index ] << "\n";
                }
                t_stream << "\n";

                continue;
            }
            if( t_in_state == stream::s_stop )
            {
                t_stream.close();
                continue;
            }
            if( t_in_state == stream::s_exit )
            {
                break;
            }
        }
        return;
    }

    void rf_ascii_consumer::finalize()
    {
        return;
    }

}
