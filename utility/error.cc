#include "error.hh"

namespace midge
{

    error::error() :
            f_message()
    {
    }
    error::error( const error& p_copy ) :
            f_message( p_copy.f_message.str() )
    {
    }
    error::~error() throw()
    {
    }

    const char* error::what() const throw()
    {
        return f_message.str().c_str();
    }

}

