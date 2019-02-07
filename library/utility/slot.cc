/*
 * slot.cc
 *
 *  Created on: Feb 6, 2019
 *      Author: N.S. Oblath
 */

#include "slot.hh"

#include "node.hh"

namespace midge
{

    slot::slot( const std::string& name, node* owner )
    {
        if( owner ) owner->slot_ptr( this, name );
    }

    slot::~slot()
    {}

} /* namespace midge */
