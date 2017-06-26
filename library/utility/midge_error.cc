#include <iostream>

#include "midge_error.hh"
using std::cout;
using std::endl;

namespace midge
{

    error::error() :
            exception(),
            f_message( "" )
    {
    }
    error::error( const error& p_copy ) :
            exception( p_copy ),
            f_message( p_copy.f_message )
    {
    }
    error& error::operator=( const error& p_copy )
    {
        exception::operator=( p_copy );
        f_message = p_copy.f_message;
        return *this;
    }
    error::~error() throw()
    {
    }

    const char* error::what() const throw()
    {
        return f_message.c_str();
    }

    node_fatal_error::node_fatal_error() :
        exception(),
        f_message( "" )
    {
    }
    node_fatal_error::node_fatal_error( const node_fatal_error& p_copy ) :
        exception( p_copy ),
        f_message( p_copy.f_message )
    {
    }
    node_fatal_error& node_fatal_error::operator=( const node_fatal_error& p_copy )
    {
        exception::operator=( p_copy );
        f_message = p_copy.f_message;
        return *this;
    }
    node_fatal_error::~node_fatal_error() throw()
    {
    }
    const char* node_fatal_error::what() const throw()
    {
        return f_message.c_str();
    }

    node_nonfatal_error::node_nonfatal_error() :
        exception(),
        f_message( "" )
    {
    }
    node_nonfatal_error::node_nonfatal_error( const node_nonfatal_error& p_copy ) :
        exception( p_copy ),
        f_message( p_copy.f_message )
    {
    }
    node_nonfatal_error& node_nonfatal_error::operator=( const node_nonfatal_error& p_copy )
    {
        exception::operator=( p_copy );
        f_message = p_copy.f_message;
        return *this;
    }
    node_nonfatal_error::~node_nonfatal_error() throw()
    {
    }
    const char* node_nonfatal_error::what() const throw()
    {
        return f_message.c_str();
    }

}

