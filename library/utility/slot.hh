/*
 * slot.hh
 *
 *  Created on: Feb 6, 2019
 *      Author: N.S. Oblath
 */

#ifndef MIDGE_UTILITY_SLOT_HH_
#define MIDGE_UTILITY_SLOT_HH_

#include <functional>
#include <string>

namespace midge
{
    class node;

    class slot
    {
        public:
            slot( const std::string& name, node* owner = nullptr );
            virtual ~slot();
    };


    template< typename... x_args >
    class m_slot : public slot
    {
        public:
            using signature = void( x_args... );

        public:
            m_slot( const std::string& name, const std::function< signature >& sig, node* owner = nullptr );
            template< typename T >
            m_slot( const std::string& name,  T *inst, void (T::*func)( x_args... ), node* owner = nullptr );
            template< typename T >
            m_slot( const std::string& name,  T *inst, void (T::*func)( x_args... ) const, node* owner = nullptr );
            m_slot( const m_slot& ) = delete;
            m_slot( m_slot&& ) = delete;

            std::function< signature >& function();

        protected:
            std::function< signature > f_function;
    };


    template< typename... x_args >
    m_slot< x_args... >::m_slot( const std::string& name, const std::function< signature >& sig, node* owner ) :
            slot( name, owner ),
            f_function( sig )
    {}

    template< typename... x_args >
    template< typename T >
    m_slot< x_args... >::m_slot( const std::string& name,  T *inst, void (T::*func)( x_args... ), node* owner ) :
            slot( name, owner ),
            f_function( [func, inst]( x_args... args ){ return (inst->*func)(args...);} )
    {}

    template< typename... x_args >
    template< typename T >
    m_slot< x_args... >::m_slot( const std::string& name,  T *inst, void (T::*func)( x_args... ) const, node* owner ) :
            slot( name, owner ),
            f_function( [func, inst]( x_args... args ){ return (inst->*func)(args...);} )
    {}

    template< typename... x_args >
    std::function< typename m_slot< x_args... >::signature >& m_slot< x_args... >::function()
    {
        return f_function;
    }


} /* namespace midge */

#endif /* MIDGE_UTILITY_SLOT_HH_ */
