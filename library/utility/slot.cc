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

    slot::slot( const string_t& name, node* owner ) :
            f_name( name )
    {
        if( owner ) owner->slot_ptr( this, name );
    }

    slot::~slot()
    {}

} /* namespace midge */
