#include "real_ascii_consumer.hh"

namespace midge
{

    real_ascii_consumer::real_ascii_consumer() :
            f_filename( "" ),
            f_filestream(),
            f_internal( 0 ),
            f_data( NULL ),
            f_size( 0 )
    {
    }
    real_ascii_consumer::~real_ascii_consumer()
    {
    }

    void real_ascii_consumer::set_filename( const string& p_filename )
    {
        f_filename = p_filename;
        return;
    }
    const string& real_ascii_consumer::get_filename() const
    {
        return f_filename;
    }

    void real_ascii_consumer::initialize_consumer()
    {
        f_filestream.open( f_filename, std::ios_base::trunc );
        f_data = input< 0 >()->data();
        f_size = input< 0 >()->size();

        return;
    }

    void real_ascii_consumer::execute_consumer()
    {
        for( uint64_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_filestream << t_index + f_internal << " " << f_data[ t_index ] << "\n";
        }

        f_internal += f_size;

        return;
    }

    void real_ascii_consumer::finalize_consumer()
    {
        f_filestream.close();
    }
}
