#ifndef _midge_consumer_hh_
#define _midge_consumer_hh_

#include "core_message.hh"

#include "node.hh"
#include "typechain.hh"
#include "typelength.hh"
#include "typeat.hh"
#include "_in.hh"

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
        public typechain< _in, x_in_list >
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
            const _stream< typename typeat< x_in_list, x_index >::result >& in_stream()
            {
                return this->_in< typename typeat< x_in_list, x_index >::result, x_index >::get_stream();
            }

//            //******
//            //action
//            //******
//
//        public:
//            void initialize();
//            bool start();
//            bool execute();
//            bool stop();
//            void finalize();
//
//        protected:
//            typedef enum
//            {
//                e_idle = 0, e_initialized = 1, e_started = 2
//            } state;
//            state f_state;
//            vector< vector< node* > > f_ins;
//
//            virtual void initialize_consumer();
//            virtual bool start_consumer();
//            virtual bool execute_consumer();
//            virtual bool stop_consumer();
//            virtual void finalize_consumer();
    };

    template< class x_type, class x_in_list >
    _consumer< x_type, x_in_list >::_consumer() :
            consumer(),
            typechain< _in, x_in_list >()
    {
    }
    template< class x_type, class x_in_list >
    _consumer< x_type, x_in_list >::~_consumer()
    {
    }

//    //******
//    //action
//    //******
//
//    template< class x_type, class x_in_list >
//    inline void _consumer< x_type, x_in_list >::initialize()
//    {
//        if( f_state == e_idle )
//        {
//            msg_debug( coremsg, "initializing consumer named <" << this->get_name() << ">" << eom );
//            f_state = e_initialized;
//
//            msg_debug( coremsg, "  initializing self" << eom );
//            initialize_consumer();
//
//            msg_debug( coremsg, "initialized consumer named <" << this->get_name() << ">" << eom );
//            return;
//        }
//
//        if( f_state == e_initialized )
//        {
//            msg_debug( coremsg, "already initialized consumer named <" << this->get_name() << ">" << eom );
//            return;
//        }
//
//        throw error() << "cannot initialize consumer named <" << this->get_name() << "> from state <" << f_state << ">";
//        return;
//    }
//    template< class x_type, class x_in_list >
//    inline bool _consumer< x_type, x_in_list >::start()
//    {
//        if( f_state == e_initialized )
//        {
//            if( ++f_calls != typelength< x_in_list >::result )
//            {
//                msg_debug( coremsg, "starting consumer named <" << this->get_name() << "> on call <" << f_calls << "> of <" << (count_t) (typelength< x_in_list >::result) << ">" << eom );
//                return true;
//            }
//            f_calls = 0;
//
//            msg_debug( coremsg, "starting consumer named <" << this->get_name() << ">" << eom );
//            f_state = e_started;
//
//            msg_debug( coremsg, "  checking ins" << eom );
//            for( count_t t_index = 0; t_index < typelength< x_in_list >::result; t_index++ )
//            {
//                if( f_ins[ t_index ] == NULL )
//                {
//                    throw error() << "consumer named <" << this->get_name() << "> cannot start with in <" << t_index << "> unset";
//                    return false;
//                }
//            }
//
//            msg_debug( coremsg, "  starting self" << eom );
//            if( start_consumer() == false )
//            {
//                msg_debug( coremsg, "    done" << eom );
//                return false;
//            }
//
//            msg_debug( coremsg, "started consumer named <" << this->get_name() << ">" << eom );
//            return true;
//        }
//
//        if( f_state == e_started )
//        {
//            msg_debug( coremsg, "already started consumer named <" << this->get_name() << ">" << eom );
//            return true;
//        }
//
//        throw error() << "consumer named <" << this->get_name() << "> cannot start from state <" << f_state << ">";
//        return false;
//    }
//    template< class x_type, class x_in_list >
//    inline bool _consumer< x_type, x_in_list >::execute()
//    {
//        if( f_state == e_started )
//        {
//            if( ++f_calls != typelength< x_in_list >::result )
//            {
//                msg_debug( coremsg, "executing consumer named <" << this->get_name() << "> on call <" << f_calls << "> of <" << (count_t) (typelength< x_in_list >::result) << ">" << eom );
//                return true;
//            }
//            f_calls = 0;
//
//            msg_debug( coremsg, "executing consumer named <" << this->get_name() << ">" << eom );
//
//            msg_debug( coremsg, "  executing self" << eom );
//            if( execute_consumer() == false )
//            {
//                msg_debug( coremsg, "    done" << eom );
//                return false;
//            }
//
//            msg_debug( coremsg, "executed consumer named <" << this->get_name() << ">" << eom );
//            return true;
//        }
//
//        throw error() << "consumer named <" << this->get_name() << "> cannot execute from state <" << f_state << ">";
//        return false;
//    }
//    template< class x_type, class x_in_list >
//    inline bool _consumer< x_type, x_in_list >::stop()
//    {
//        if( f_state == e_started )
//        {
//            if( ++f_calls != typelength< x_in_list >::result )
//            {
//                msg_debug( coremsg, "stopping consumer named <" << this->get_name() << "> on call <" << f_calls << "> of <" << (count_t) (typelength< x_in_list >::result) << ">" << eom );
//                return true;
//            }
//            f_calls = 0;
//
//            msg_debug( coremsg, "stopping consumer named <" << this->get_name() << ">" << eom );
//            f_state = e_initialized;
//
//            msg_debug( coremsg, "  stopping self" << eom );
//            if( stop_consumer() == false )
//            {
//                msg_debug( coremsg, "    done" << eom );
//                return false;
//            }
//
//            msg_debug( coremsg, "stopped consumer named <" << this->get_name() << ">" << eom );
//            return true;
//        }
//
//        if( f_state == e_initialized )
//        {
//            msg_debug( coremsg, "already stopped consumer named <" << this->get_name() << ">" << eom );
//            return true;
//        }
//
//        throw error() << "consumer named <" << this->get_name() << "> cannot stop from state <" << f_state << ">";
//        return false;
//    }
//    template< class x_type, class x_in_list >
//    inline void _consumer< x_type, x_in_list >::finalize()
//    {
//        if( f_state == e_initialized )
//        {
//            msg_debug( coremsg, "finalizing consumer named <" << this->get_name() << ">" << eom );
//            f_state = e_idle;
//
//            msg_debug( coremsg, "  finalizing self" << eom );
//            finalize_consumer();
//
//            msg_debug( coremsg, "finalized consumer named <" << this->get_name() << ">" << eom );
//            return;
//        }
//
//        if( f_state == e_idle )
//        {
//            msg_debug( coremsg, "already finalized consumer named <" << this->get_name() << ">" << eom );
//            return;
//        }
//
//        throw error() << "consumer named <" << this->get_name() << "> cannot finalize from state <" << f_state << ">";
//        return;
//    }
//
//    template< class x_type, class x_in_list >
//    inline void _consumer< x_type, x_in_list >::initialize_consumer()
//    {
//        return;
//    }
//    template< class x_type, class x_in_list >
//    inline bool _consumer< x_type, x_in_list >::start_consumer()
//    {
//        return true;
//    }
//    template< class x_type, class x_in_list >
//    inline bool _consumer< x_type, x_in_list >::execute_consumer()
//    {
//        return true;
//    }
//    template< class x_type, class x_in_list >
//    inline bool _consumer< x_type, x_in_list >::stop_consumer()
//    {
//        return true;
//    }
//    template< class x_type, class x_in_list >
//    inline void _consumer< x_type, x_in_list >::finalize_consumer()
//    {
//        return;
//    }

}

#endif
