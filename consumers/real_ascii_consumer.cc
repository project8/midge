#include "real_ascii_consumer.hh"

namespace midge
{

    real_ascii_consumer::real_ascii_consumer() :
            f_file( "" ),
            f_stream(),
            f_internal( 0 ),
            f_raw( NULL ),
            f_size( 0 )
    {
    }
    real_ascii_consumer::~real_ascii_consumer()
    {
    }

    void real_ascii_consumer::set_file( const string& p_file )
    {
        f_file = p_file;
        return;
    }
    const string& real_ascii_consumer::get_file() const
    {
        return f_file;
    }

    void real_ascii_consumer::initialize_consumer()
    {
        f_stream.open( f_file, std::ios_base::trunc );
        f_raw = input< 0 >()->raw();
        f_size = input< 0 >()->get_size();

        return;
    }

    void real_ascii_consumer::execute_consumer()
    {
        for( uint64_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_stream << t_index + f_internal << " " << f_raw[ t_index ] << "\n";
        }

        f_internal += f_size;

        return;
    }

    void real_ascii_consumer::finalize_consumer()
    {
        f_stream.close();
    }
}
