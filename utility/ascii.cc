#include "ascii.hh"

namespace midge
{

    ascii::ascii() :
            f_fstream()
    {
    }
    ascii::~ascii()
    {
    }

    void ascii::read( const string& p_ascii )
    {
        f_fstream.open( p_ascii.c_str(), std::fstream::in );
        return;
    }

    void ascii::write( const string& p_ascii )
    {
        f_fstream.open( p_ascii.c_str(), std::fstream::out );
        return;
    }

    void ascii::close()
    {
        f_fstream.close();
        return;
    }

}
