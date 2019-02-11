/*
 * signal_slot_impl.hh
 *
 *  Created on: Feb 9, 2019
 *      Author: N.S. Oblath
 *
 *  This file includes implementation of the signal and slot member functions
 *  that depend on one another
 */

#ifndef MIDGE_SIGNAL_SLOT_IMPL_HH_
#define MIDGE_SIGNAL_SLOT_IMPL_HH_

#include "signal.hh"
#include "slot.hh"

namespace midge
{
    //********************
    // m_signal
    //********************

    template< typename... x_args >
    unsigned m_signal< x_args... >::connect( slot* p_slot )
    {
        // ensure that the slot is of the correct type
        m_slot< x_args... >* derived_slot = dynamic_cast< m_slot< x_args... >* >( p_slot );
        if( p_slot == nullptr )
        {
            throw error() << "signal/slot signature mismatch: signal=" << f_name << "; slot=" << p_slot->name();
        }

        // make the connection
        int connection_id = connect_function( derived_slot->function() );

        // inform the slot of the signal connection so that it can disconnect
        derived_slot->connections().insert( std::pair< int, m_signal< x_args... >* >( connection_id, this ) );

        return connection_id;
    }

    //********************
    // m_slot
    //********************

    template< typename... x_args >
    void m_slot< x_args... >::disconnect_all()
    {
        for( auto connection : f_connections )
        {
            connection.second->disconnect( connection.first );
        }
        return;
    }


}

#endif /* MIDGE_SIGNAL_SLOT_IMPL_HH_ */
