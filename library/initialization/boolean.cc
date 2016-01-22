#include "boolean.hh"

#include <sstream>

using std::string;
using std::stringstream;

namespace midge
{

    boolean::boolean( const string& p_string ) :
            f_string( p_string ),
            f_value( 0. )
    {
        set( this );

        stringstream t_stream;
        t_stream << f_string;
        t_stream >> f_value;
    }
    boolean::~boolean()
    {
    }
    boolean* boolean::clone() const
    {
        return (new boolean( f_string ));
    }

    string& boolean::str()
    {
        return f_string;
    }
    const string& boolean::str() const
    {
        return f_string;
    }

    bool& boolean::get()
    {
        return f_value;
    }
    const bool& boolean::get() const
    {
        return f_value;
    }

}
