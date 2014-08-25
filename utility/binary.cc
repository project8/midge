#include "binary.hh"

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

}
