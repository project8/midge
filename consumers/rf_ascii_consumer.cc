#include "rf_ascii_consumer.hh"

namespace midge
{

    rf_ascii_consumer::rf_ascii_consumer() :
            f_file( "" ),
            f_stream(),
            f_size( 0 ),
            f_interval( 1. ),
            f_center_time( 0. ),
            f_in( NULL )
    {
    }
    rf_ascii_consumer::~rf_ascii_consumer()
    {
    }

    void rf_ascii_consumer::set_file( const string& p_file )
    {
        f_file = p_file;
        return;
    }
    const string& rf_ascii_consumer::get_file() const
    {
        return f_file;
    }

    void rf_ascii_consumer::initialize_consumer()
    {
        f_stream.open( f_file, std::ios_base::trunc );

        return;
    }

    void rf_ascii_consumer::start_consumer()
    {
        f_size = in< 0 >()->get_size();
        f_interval = in< 0 >()->get_interval();
        f_center_time = in< 0 >()->get_center_time();
        f_in = in< 0 >()->raw();

        return;
    }

    void rf_ascii_consumer::execute_consumer()
    {
        for( count_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_stream << f_center_time << " " << (t_index * f_interval) << " " << f_in[ t_index ] << "\n";
        }

        return;
    }

    void rf_ascii_consumer::stop_consumer()
    {
        f_size = 0;
        f_interval = 1.;
        f_center_time = 0.;
        f_in = NULL;

        return;
    }

    void rf_ascii_consumer::finalize_consumer()
    {
        f_stream.close();

        return;
    }
}
