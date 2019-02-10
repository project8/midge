/*
 * slot.hh
 *
 *  Created on: Feb 6, 2019
 *      Author: N.S. Oblath
 */

#ifndef MIDGE_UTILITY_SLOT_HH_
#define MIDGE_UTILITY_SLOT_HH_

#include "types.hh"

#include "member_variables.hh"

#include <functional>
#include <set>

namespace midge
{
    template< typename... x_args >
    class m_signal;

    class slot
    {
        public:
            slot( const string_t& p_name );
            template< typename x_owner >
            slot( const string_t& p_name, x_owner* p_owner );
            virtual ~slot();

            mv_referrable( string_t, name );
    };


    template< typename... x_args >
    class m_slot : public slot
    {
        public:
            using signature = void( x_args... );

        public:
            m_slot( const string_t& name, const std::function< signature >& sig );
            template< typename T >
            m_slot( const string_t& name,  T *inst, void (T::*func)( x_args... ) );
            template< typename T >
            m_slot( const string_t& name,  T *inst, void (T::*func)( x_args... ) const );
            template< typename x_owner =  void >
            m_slot( const string_t& name, const std::function< signature >& sig, x_owner* owner );
            template< typename T, typename x_owner = void >
            m_slot( const string_t& name,  T *inst, void (T::*func)( x_args... ), x_owner* owner );
            template< typename T, typename x_owner = void >
            m_slot( const string_t& name,  T *inst, void (T::*func)( x_args... ) const, x_owner* owner );
            m_slot( const m_slot& ) = delete;
            m_slot( m_slot&& ) = delete;
            virtual ~m_slot();

            void disconnect_all();

            mv_referrable( std::function< signature >, function );

            typedef std::set< std::pair< int, m_signal< x_args... >* > > signal_connections; // to get around the problem of having a comma inside a macro function argument
            mv_referrable( signal_connections, connections );

    };


    template< typename x_owner >
    slot::slot( const string_t& p_name, x_owner* p_owner ) :
            f_name( p_name )
    {
        if( p_owner ) p_owner->slot_ptr( this, p_name );
    }


    template< typename... x_args >
    m_slot< x_args... >::m_slot( const string_t& name, const std::function< signature >& sig ) :
            slot( name ),
            f_function( sig )
    {}

    template< typename... x_args >
    template< typename T >
    m_slot< x_args... >::m_slot( const string_t& name,  T *inst, void (T::*func)( x_args... ) ) :
            slot( name ),
            f_function( [func, inst]( x_args... args ){ return (inst->*func)(args...);} )
    {}

    template< typename... x_args >
    template< typename T >
    m_slot< x_args... >::m_slot( const string_t& name,  T *inst, void (T::*func)( x_args... ) const ) :
            slot( name ),
            f_function( [func, inst]( x_args... args ){ return (inst->*func)(args...);} )
    {}

    template< typename... x_args >
    template< typename x_owner >
    m_slot< x_args... >::m_slot( const string_t& name, const std::function< signature >& sig, x_owner* owner ) :
            slot( name, owner ),
            f_function( sig )
    {}

    template< typename... x_args >
    template< typename T, typename x_owner >
    m_slot< x_args... >::m_slot( const string_t& name,  T *inst, void (T::*func)( x_args... ), x_owner* owner ) :
            slot( name, owner ),
            f_function( [func, inst]( x_args... args ){ return (inst->*func)(args...);} )
    {}

    template< typename... x_args >
    template< typename T, typename x_owner >
    m_slot< x_args... >::m_slot( const string_t& name,  T *inst, void (T::*func)( x_args... ) const, x_owner* owner ) :
            slot( name, owner ),
            f_function( [func, inst]( x_args... args ){ return (inst->*func)(args...);} )
    {}

    template< typename... x_args >
    m_slot< x_args... >::~m_slot< x_args... >()
    {
        disconnect_all();
    }

} /* namespace midge */

#endif /* MIDGE_UTILITY_SLOT_HH_ */
