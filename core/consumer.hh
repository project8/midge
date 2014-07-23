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

    template< class x_type, class x_in_list >
    class _consumer :
        public consumer,
        public typechain< x_in_list, _in >
    {
        public:
            using node::in;

        public:
            _consumer();
            virtual ~_consumer();

            //***********
            //composition
            //***********

        protected:
            template< int x_index >
            typename typeat< x_in_list, x_index >::result* in()
            {
                return this->_in< typename typeat< x_in_list, x_index >::result, x_index >::get();
            }

            //******
            //action
            //******

        public:
            void initialize();
            bool start();
            bool execute();
            bool stop();
            void finalize();

        protected:
            typedef enum
            {
                e_idle = 0, e_initialized = 1, e_started = 2
            } state;
            state f_state;
            count_t f_calls;
            node* f_ins[ typelength< x_in_list >::result ];

            virtual void initialize_consumer();
            virtual bool start_consumer();
            virtual bool execute_consumer();
            virtual bool stop_consumer();
            virtual void finalize_consumer();
    };

    template< class x_type, class x_in_list >
    _consumer< x_type, x_in_list >::_consumer() :
            consumer(),
            typechain< x_in_list, _in >( f_ins ),
            f_state( e_idle ),
            f_calls( 0 )
    {
        for( count_t t_index = 0; t_index < typelength< x_in_list >::result; t_index++ )
        {
            f_ins[ t_index ] = NULL;
        }
    }
    template< class x_type, class x_in_list >
    _consumer< x_type, x_in_list >::~_consumer()
    {
    }

    //******
    //action
    //******

    template< class x_type, class x_in_list >
    inline void _consumer< x_type, x_in_list >::initialize()
    {
        if( f_state == e_idle )
        {
            f_state = e_initialized;

            initialize_consumer();
        }

        if( f_state != e_initialized )
        {
            throw error() << "consumer named <" << this->get_name() << "> cannot initialize from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_in_list >
    inline bool _consumer< x_type, x_in_list >::start()
    {
        if( f_state == e_initialized )
        {
            if( ++f_calls != typelength< x_in_list >::result )
            {
                return true;
            }
            f_calls = 0;

            f_state = e_started;

            for( count_t t_index = 0; t_index < typelength< x_in_list >::result; t_index++ )
            {
                if( f_ins[ t_index ] == NULL )
                {
                    throw error() << "consumer named <" << this->get_name() << "> cannot start with in <" << t_index << "> unset";
                    return false;
                }
            }

            if( start_consumer() == false )
            {
                return false;
            }
        }

        if( f_state != e_started )
        {
            throw error() << "consumer named <" << this->get_name() << "> cannot start from state <" << f_state << ">";
            return false;
        }

        return true;
    }
    template< class x_type, class x_in_list >
    inline bool _consumer< x_type, x_in_list >::execute()
    {
        if( f_state == e_started )
        {
            if( ++f_calls != typelength< x_in_list >::result )
            {
                return true;
            }
            f_calls = 0;

            if( execute_consumer() == false )
            {
                return false;
            }
        }
        else
        {
            throw error() << "consumer named <" << this->get_name() << "> cannot execute from state <" << f_state << ">";
            return false;
        }

        return true;
    }
    template< class x_type, class x_in_list >
    inline bool _consumer< x_type, x_in_list >::stop()
    {
        if( f_state == e_started )
        {
            if( ++f_calls != typelength< x_in_list >::result )
            {
                return true;
            }
            f_calls = 0;

            f_state = e_initialized;

            if( stop_consumer() == false )
            {
                return false;
            }
        }

        if( f_state != e_initialized )
        {
            throw error() << "consumer named <" << this->get_name() << "> cannot stop from state <" << f_state << ">";
            return false;
        }

        return true;
    }
    template< class x_type, class x_in_list >
    inline void _consumer< x_type, x_in_list >::finalize()
    {
        if( f_state == e_initialized )
        {
            f_state = e_idle;

            finalize_consumer();
        }

        if( f_state != e_idle )
        {
            throw error() << "consumer named <" << this->get_name() << "> cannot finalize from state <" << f_state << ">";
        }

        return;
    }

    template< class x_type, class x_in_list >
    inline void _consumer< x_type, x_in_list >::initialize_consumer()
    {
        return;
    }
    template< class x_type, class x_in_list >
    inline bool _consumer< x_type, x_in_list >::start_consumer()
    {
        return true;
    }
    template< class x_type, class x_in_list >
    inline bool _consumer< x_type, x_in_list >::execute_consumer()
    {
        return true;
    }
    template< class x_type, class x_in_list >
    inline bool _consumer< x_type, x_in_list >::stop_consumer()
    {
        return true;
    }
    template< class x_type, class x_in_list >
    inline void _consumer< x_type, x_in_list >::finalize_consumer()
    {
        return;
    }

}

#endif
