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
    signal::signal( const std::string& name, node* owner )
    {
        if( owner ) owner->signal_ptr( this, name );
    }

    signal::~signal()
    {}
}



