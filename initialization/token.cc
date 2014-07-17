#include "token.hh"

namespace midge
{
    token::token() :
            f_value( "" )
    {
    }
    token::token( const string& p_value ) :
            f_value( p_value )
    {
    }
    token::token( const token& p_copy ) :
            f_value( p_copy.f_value )
    {
    }
    token::~token()
    {
    }

    string& token::str()
    {
        return f_value;
    }
    const string& token::str() const
    {
        return f_value;
    }

}
