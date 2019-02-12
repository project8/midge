/*
 * m_signal.hh
 *
 *  Created on: Dec 18, 2018
 *      Adapted by: N.S. Oblath
 *      Original author: Simon Schneegans
 *      License: public domain
 *      URL: http://simmesimme.github.io/tutorials/2015/09/20/m_signal-slot
 */

#ifndef MIDGE_SIGNAL_HH_
#define MIDGE_SIGNAL_HH_

#include "coremsg.hh"
#include "midge_error.hh"
#include "types.hh"

#include "member_variables.hh"

#include <functional>
#include <map>

namespace midge
{
    class slot;

    class signal
    {
        public:
            signal( const string_t& p_name );
            template< typename x_owner >
            signal( const string_t& p_name, x_owner* p_owner );
            virtual ~signal();

            virtual unsigned connect( slot* p_slot ) = 0;

            mv_referrable( string_t, name );
    };

    /// A m_signal object may call multiple slots with the
    /// same signature. You can connect functions to the m_signal
    /// which will be called when the emit() method on the
    /// m_signal object is invoked. Any argument passed to emit()
    /// will be passed to the given functions.
    /// The name "m_signal" was chosen to avoid conflicting with std::signal.
    template< typename... x_args >
    class m_signal : public signal
    {
        public:
            using signature = void( x_args... );

        public:
            m_signal( const string_t& p_name );
            template< typename x_owner = void >
            m_signal( const string_t& p_name, x_owner* p_owner );
            m_signal( const m_signal& ) = delete;
            m_signal( m_signal&& ) = delete;
            virtual ~m_signal();

            virtual unsigned connect( slot* p_slot );

            // connects a member function to this Signal
            template< typename T >
            unsigned connect_function( T *inst, void (T::*func)( x_args... ) );

            // connects a const member function to this Signal
            template< typename T >
            unsigned connect_function( T *inst, void (T::*func)( x_args... ) const );

            // connects a std::function to the m_signal. The returned
            // value can be used to disconnect the function again
            unsigned connect_function( const std::function< signature > & slot ) const;

            // disconnects a previously connected function
            void disconnect( unsigned id ) const;

            // disconnects all previously connected functions
            void disconnect_all() const;

            // calls all connected functions
            void emit( x_args... args );
            void operator()( x_args... args );

            // assignment creates new Signal
            m_signal& operator=( m_signal const& other );

        private:
            typedef std::map< unsigned, std::function< signature > > slot_map; // to get around the problem of having a comma inside a macro function argument
            mv_referrable_mutable_const( slot_map, slots );
            mv_accessible_mutable_noset( unsigned, current_id );
    };


    template< typename x_owner >
    signal::signal( const string_t& p_name, x_owner* p_owner ) :
            f_name( p_name )
    {
        if( p_owner ) p_owner->signal_ptr( this, p_name );
    }


    template< typename... x_args >
    m_signal< x_args... >::m_signal( const string_t& name ) :
            signal( name ),
            f_slots(),
            f_current_id( 0 )
    {}

    template< typename... x_args >
    template< typename x_owner >
    m_signal< x_args... >::m_signal( const string_t& name, x_owner* owner ) :
            signal( name, owner ),
            f_slots(),
            f_current_id( 0 )
    {}

    template< typename... x_args >
    m_signal< x_args... >::~m_signal()
    {
        disconnect_all();
    }

    // connects a member function to this Signal
    template< typename... x_args >
    template< typename T >
    unsigned m_signal< x_args... >::connect_function( T *inst, void (T::*func)( x_args... ) )
    {
        return connect( [=]( x_args... args )
        {
            (inst->*func)( args... );
        });
    }

    // connects a const member function to this Signal
    template< typename... x_args >
    template< typename T >
    unsigned m_signal< x_args... >::connect_function( T *inst, void (T::*func)( x_args... ) const )
    {
        return connect( [=]( x_args... args )
        {
            (inst->*func)( args... );
        });
    }

    // connects a std::function to the m_signal. The returned
    // value can be used to disconnect the function again
    template< typename... x_args >
    unsigned m_signal< x_args... >::connect_function( std::function< signature > const& slot ) const
    {
        f_slots.insert( std::make_pair( ++f_current_id, slot ) );
        return f_current_id;
    }

    // disconnects a previously connected function
    template< typename... x_args >
    void m_signal< x_args... >::disconnect( unsigned id ) const
    {
        f_slots.erase( id );
    }

    // disconnects all previously connected functions
    template< typename... x_args >
    void m_signal<  x_args... >::disconnect_all() const
    {
        f_slots.clear();
    }

    // calls all connected functions
    template< typename... x_args >
    void m_signal< x_args... >::emit( x_args... args )
    {
        for( auto it : f_slots )
        {
            it.second( args... );
        }
    }

    template< typename... x_args >
    void m_signal< x_args... >::operator()( x_args... args )
    {
        emit( args... );
    }

}

#endif /* MIDGE_SIGNAL_HH_ */
