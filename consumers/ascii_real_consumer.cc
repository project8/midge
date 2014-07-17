#include "ascii_real_consumer.hh"

namespace midge
{

    ascii_real_consumer::ascii_real_consumer() :
            f_filename( "" ),
            f_filestream(),
            f_internal( 0 )
    {
    }
    ascii_real_consumer::~ascii_real_consumer()
    {
    }

    void ascii_real_consumer::set_filename( const string& p_filename )
    {
        f_filename = p_filename;
        return;
    }
    const string& ascii_real_consumer::get_filename() const
    {
        return f_filename;
    }

    void ascii_real_consumer::initialize_consumer()
    {
        f_filestream.open( f_filename, std::ios_base::trunc );
        return;
    }

    void ascii_real_consumer::execute_consumer()
    {
        uint64_t t_size = input< 0 >()->get_size();
        real_t* t_data = input< 0 >()->data();

        for( uint64_t t_index = 0; t_index < t_size; t_index++ )
        {
            f_filestream << t_index + f_internal << " " << t_data[ t_index ] << "\n";
        }

        f_internal += t_size;

        return;
    }

    void ascii_real_consumer::finalize_consumer()
    {
        f_filestream.close();
    }
}
