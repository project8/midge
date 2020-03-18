#include <iostream>

#include "midge_error.hh"

namespace midge
{

    error::error() :
            base_exception()
    {
    }

    error::error( const error& p_copy ) :
            base_exception( p_copy )
    {
    }

    error& error::operator=( const error& p_copy )
    {
        base_exception::operator=( p_copy );
        return *this;
    }

    error::~error() noexcept
    {
    }


    node_fatal_error::node_fatal_error() :
            base_exception()
    {
    }

    node_fatal_error::node_fatal_error( const node_fatal_error& p_copy ) :
            base_exception( p_copy )
    {
    }

    node_fatal_error& node_fatal_error::operator=( const node_fatal_error& p_copy )
    {
        base_exception::operator=( p_copy );
        return *this;
    }

    node_fatal_error::~node_fatal_error() noexcept
    {
    }


    node_nonfatal_error::node_nonfatal_error() :
        base_exception()
    {
    }

    node_nonfatal_error::node_nonfatal_error( const node_nonfatal_error& p_copy ) :
        base_exception( p_copy )
    {
    }

    node_nonfatal_error& node_nonfatal_error::operator=( const node_nonfatal_error& p_copy )
    {
        base_exception::operator=( p_copy );
        return *this;
    }

    node_nonfatal_error::~node_nonfatal_error() noexcept
    {
    }

}

