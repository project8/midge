#include "binary.hh"

#include "message_logger.hh"
#include "message_logger.hh"

using std::string;

namespace midge
{

    binary::binary() :
            f_fstream()
    {
    }
    binary::~binary()
    {
    }

    void binary::read( const string& p_binary )
    {
        f_fstream.open( p_binary.c_str(), std::fstream::in | std::ios::binary );
        return;
    }

    void binary::write( const string& p_binary )
    {
        f_fstream.open( p_binary.c_str(), std::fstream::out | std::ios::binary );
        return;
    }

    void binary::close()
    {
        f_fstream.close();
        return;
    }

    bool binary::end()
    {
        f_fstream.peek();
        if( f_fstream.good() == false )
        {
            return true;
        }
        return false;
    }

}
