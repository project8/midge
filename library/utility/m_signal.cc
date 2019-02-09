/*
 * m_signal.cc
 *
 *  Created on: Feb 6, 2019
 *      Author: obla999
 */

#include "m_signal.hh"

#include "node.hh"

namespace midge
{
    signal::signal( const string_t& name, node* owner ) :
            f_name( name )
    {
        if( owner ) owner->signal_ptr( this, name );
    }

    signal::~signal()
    {}
}



