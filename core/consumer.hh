#ifndef _midge_consumer_hh_
#define _midge_consumer_hh_

#include "node.hh"
#include "in.hh"
#include "typechain.hh"
#include "typelength.hh"
#include "typeat.hh"

#include <vector>
using std::vector;

namespace midge
{

    class consumer :
        virtual public node
    {
        protected:
            consumer();

        public:
            virtual ~consumer();
    };

    template< class x_type, class x_input_list >
    class _consumer :
        public consumer,
        public typechain< x_input_list, _in >
    {
        public:
            using node::input;

        public:
            _consumer();
            virtual ~_consumer();

            //***********
            //composition
            //***********

        protected:
            template< int x_index >
            typename typeat< x_input_list, x_index >::result* input()
            {
                return this->_in< typename typeat< x_input_list, x_index >::result, x_index >::get();
            }

            //******
            //action
            //******

        public:
            void initialize();
            void execute();
            void finalize();

        protected:
            typedef enum
            {
                e_idle = 0, e_initialized = 1
            } state;
            state f_state;
            uint64_t f_count;
            node* f_inputs[ typelength< x_input_list >::result ];

            virtual void initialize_consumer();
            virtual void execute_consumer();
            virtual void finalize_consumer();
    };

    template< class x_type, class x_input_list >
    _consumer< x_type, x_input_list >::_consumer() :
            consumer(),
            typechain< x_input_list, _in >( f_inputs ),
            f_state( e_idle ),
            f_count( 0 )
    {
        for( uint64_t t_index = 0; t_index < typelength< x_input_list >::result; t_index++ )
        {
            f_inputs[ t_index ] = NULL;
        }
    }
    template< class x_type, class x_input_list >
    _consumer< x_type, x_input_list >::~_consumer()
    {
    }

    //******
    //action
    //******

    template< class x_type, class x_input_list >
    inline void _consumer< x_type, x_input_list >::initialize()
    {
        if( f_state == e_idle )
        {
            if( ++f_count != typelength< x_input_list >::result )
            {
                return;
            }

            f_count = 0;
            f_state = e_initialized;

            for( uint64_t t_index = 0; t_index < typelength< x_input_list >::result; t_index++ )
            {
                if( f_inputs[ t_index ] == NULL )
                {
                    throw error() << "consumer named <" << this->get_name() << "> cannot initialize with input <" << t_index << "> unset";
                }
            }

            initialize_consumer();
        }

        if( f_state != e_initialized )
        {
            throw error() << "consumer named <" << this->get_name() << "> cannot initialize from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_input_list >
    inline void _consumer< x_type, x_input_list >::execute()
    {
        if( f_state == e_initialized )
        {
            if( ++f_count != typelength< x_input_list >::result )
            {
                return;
            }

            f_count = 0;

            execute_consumer();
        }
        else
        {
            throw error() << "consumer named <" << this->get_name() << "> cannot execute from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_input_list >
    inline void _consumer< x_type, x_input_list >::finalize()
    {
        if( f_state == e_initialized )
        {
            if( ++f_count != typelength< x_input_list >::result )
            {
                return;
            }

            f_count = 0;
            f_state = e_idle;

            finalize_consumer();
        }

        if( f_state != e_idle )
        {
            throw error() << "consumer named <" << this->get_name() << "> cannot finalize from state <" << f_state << ">";
        }

        return;
    }

    template< class x_type, class x_input_list >
    inline void _consumer< x_type, x_input_list >::initialize_consumer()
    {
        return;
    }
    template< class x_type, class x_input_list >
    inline void _consumer< x_type, x_input_list >::execute_consumer()
    {
        return;
    }
    template< class x_type, class x_input_list >
    inline void _consumer< x_type, x_input_list >::finalize_consumer()
    {
        return;
    }

}

#endif
