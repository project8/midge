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

#include "member_variables.hh"

#include <functional>
#include <map>

namespace midge
{
    class node;

    class signal
    {
        protected:
            signal( const std::string& name, node* owner = nullptr );

        public:
            virtual ~signal();
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
            m_signal( const std::string& name, node* owner = nullptr );
            m_signal( const m_signal& ) = delete;
            m_signal( m_signal&& ) = delete;

            // connects a member function to this Signal
            template< typename T >
            int connect_member( T *inst, void (T::*func)( x_args... ) );

            // connects a const member function to this Signal
            template< typename T >
            int connect_member( T *inst, void (T::*func)( x_args... ) const );

            // connects a std::function to the m_signal. The returned
            // value can be used to disconnect the function again
            int connect( const std::function< signature > & slot ) const;

            // disconnects a previously connected function
            void disconnect( int id ) const;

            // disconnects all previously connected functions
            void disconnect_all() const;

            // calls all connected functions
            void emit( x_args... args );

            // assignment creates new Signal
            m_signal& operator=( m_signal const& other );

        private:
            typedef std::map< unsigned, std::function< signature > > slot_map; // to get around the problem of having a comma inside a macro function argument
            mv_referrable_mutable_const( slot_map, slots );
            mv_accessible_mutable_noset( unsigned, current_id );
    };

    template< typename... x_args >
    m_signal< x_args... >::m_signal( const std::string& name, node* owner ) :
            signal( name, owner ),
            f_slots(),
            f_current_id( 0 )
    {}

    // connects a member function to this Signal
    template< typename... x_args >
    template< typename T >
    int m_signal< x_args... >::connect_member( T *inst, void (T::*func)( x_args... ) )
    {
        return connect( [=]( x_args... args )
        {
            (inst->*func)( args... );
        });
    }

    // connects a const member function to this Signal
    template< typename... x_args >
    template< typename T >
    int m_signal< x_args... >::connect_member( T *inst, void (T::*func)( x_args... ) const )
    {
        return connect( [=]( x_args... args )
        {
            (inst->*func)( args... );
        });
    }

    // connects a std::function to the m_signal. The returned
    // value can be used to disconnect the function again
    template< typename... x_args >
    int m_signal< x_args... >::connect( std::function< signature > const& slot ) const
    {
        f_slots.insert( std::make_pair( ++f_current_id, slot ) );
        return f_current_id;
    }

    // disconnects a previously connected function
    template< typename... x_args >
    void m_signal< x_args... >::disconnect( int id ) const
    {
        f_slots.erase( id );
    }

    // disconnects all previously connected functions
    template< typename... x_args >
    void m_signal< x_args... >::disconnect_all() const
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

}

#endif /* MIDGE_SIGNAL_HH_ */
