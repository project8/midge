#include "ascii.hh"

using std::string;

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

    bool ascii::end()
    {
        int t_char;
        while( true )
        {
            t_char = f_fstream.peek();

            if( t_char == ' ' )
            {
                t_char = f_fstream.get();
                continue;
            }

            if( t_char == '\n' )
            {
                t_char = f_fstream.get();
                continue;
            }

            if( t_char == '\r' )
            {
                t_char = f_fstream.get();
                continue;
            }

            if( t_char == '\t' )
            {
                t_char = f_fstream.get();
                continue;
            }

            break;
        }
        if( f_fstream.good() == false )
        {
            return true;
        }
        return false;
    }

}
