#ifndef _midge_producer_hh_
#define _midge_producer_hh_

#include "core_message.hh"

#include "node.hh"
#include "out.hh"
#include "typechain.hh"
#include "typelength.hh"
#include "typeat.hh"

#include <vector>
using std::vector;

namespace midge
{

    class producer :
        virtual public node
    {
        protected:
            producer();

        public:
            virtual ~producer();
    };

    template< class x_type, class x_out_list >
    class _producer :
        public producer,
        public typechain< x_out_list, _out >
    {
        public:
            using node::out;

        public:
            _producer();
            virtual ~_producer();

            //***********
            //composition
            //***********

        protected:
            template< int x_index >
            typename typeat< x_out_list, x_index >::result* out()
            {
                return this->_out< typename typeat< x_out_list, x_index >::result, x_index >::get();
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
            node* f_outs[ typelength< x_out_list >::result ];

            virtual void initialize_producer();
            virtual bool start_producer();
            virtual bool execute_producer();
            virtual bool stop_producer();
            virtual void finalize_producer();
    };

    template< class x_type, class x_out_list >
    _producer< x_type, x_out_list >::_producer() :
            producer(),
            typechain< x_out_list, _out >( f_outs ),
            f_state( e_idle )
    {
        for( count_t t_index = 0; t_index < typelength< x_out_list >::result; t_index++ )
        {
            f_outs[ t_index ] = NULL;
        }
    }
    template< class x_type, class x_out_list >
    _producer< x_type, x_out_list >::~_producer()
    {
    }

    //******
    //action
    //******

    template< class x_type, class x_out_list >
    inline void _producer< x_type, x_out_list >::initialize()
    {
        if( f_state == e_idle )
        {
            msg_debug( coremsg, "initializing producer named <" << this->get_name() << ">" << eom );
            f_state = e_initialized;

            msg_debug( coremsg, "  initializing self" << eom );
            initialize_producer();

            msg_debug( coremsg, "initialized producer named <" << this->get_name() << ">" << eom );
            return;
        }

        if( f_state == e_initialized )
        {
            msg_debug( coremsg, "already initialized producer named <" << this->get_name() << ">" << eom );
            return;
        }

        throw error() << "cannot initialize producer named <" << this->get_name() << "> from state <" << f_state << ">";
        return;
    }
    template< class x_type, class x_out_list >
    inline bool _producer< x_type, x_out_list >::start()
    {
        if( f_state == e_initialized )
        {
            msg_debug( coremsg, "starting producer named <" << this->get_name() << ">" << eom );
            f_state = e_started;

            msg_debug( coremsg, "  checking outs" << eom );
            for( count_t t_index = 0; t_index < typelength< x_out_list >::result; t_index++ )
            {
                if( f_outs[ t_index ] == NULL )
                {
                    throw error() << "cannot start producer named <" << this->get_name() << "> with out <" << t_index << "> unset";
                    return false;
                }
            }

            msg_debug( coremsg, "  starting self" << eom );
            if( start_producer() == false )
            {
                msg_debug( coremsg, "    done" << eom );
                return false;
            }

            msg_debug( coremsg, "  starting outs" << eom );
            for( count_t t_index = 0; t_index < typelength< x_out_list >::result; t_index++ )
            {
                msg_debug( coremsg, "  starting out at <" << t_index << ">" << eom );
                if( f_outs[ t_index ]->start() == false )
                {
                    msg_debug( coremsg, "    done" << eom );
                    return false;
                }
            }

            msg_debug( coremsg, "started producer named <" << this->get_name() << ">" << eom );
            return true;
        }

        if( f_state == e_started )
        {
            msg_debug( coremsg, "already started producer named <" << this->get_name() << ">" << eom );
            return true;
        }

        throw error() << "cannot start producer named <" << this->get_name() << "> from state <" << f_state << ">";
        return false;
    }
    template< class x_type, class x_out_list >
    inline bool _producer< x_type, x_out_list >::execute()
    {
        if( f_state == e_started )
        {
            msg_debug( coremsg, "executing producer named <" << this->get_name() << ">" << eom );

            msg_debug( coremsg, "  executing self" << eom );
            if( execute_producer() == false )
            {
                msg_debug( coremsg, "    done" << eom );
                return false;
            }

            msg_debug( coremsg, "  executing outs" << eom );
            for( count_t t_index = 0; t_index < typelength< x_out_list >::result; t_index++ )
            {
                msg_debug( coremsg, "  executing out at <" << t_index << ">" << eom );
                if( f_outs[ t_index ]->execute() == false )
                {
                    msg_debug( coremsg, "    done" << eom );
                    return false;
                }
            }

            msg_debug( coremsg, "executed producer named <" << this->get_name() << ">" << eom );
            return true;
        }

        throw error() << "producer named <" << this->get_name() << "> cannot execute from state <" << f_state << ">";
        return false;
    }
    template< class x_type, class x_out_list >
    inline bool _producer< x_type, x_out_list >::stop()
    {
        if( f_state == e_started )
        {
            msg_debug( coremsg, "stopping producer named <" << this->get_name() << ">" << eom );
            f_state = e_initialized;

            msg_debug( coremsg, "  stopping self" << eom );
            if( stop_producer() == false )
            {
                msg_debug( coremsg, "    done" << eom );
                return false;
            }

            msg_debug( coremsg, "  stopping outs" << eom );
            for( count_t t_index = 0; t_index < typelength< x_out_list >::result; t_index++ )
            {
                msg_debug( coremsg, "  stopping out at <" << t_index << ">" << eom );
                if( f_outs[ t_index ]->stop() == false )
                {
                    msg_debug( coremsg, "    done" << eom );
                    return false;
                }
            }

            msg_debug( coremsg, "stopped producer named <" << this->get_name() << ">" << eom );
            return true;
        }

        if( f_state == e_initialized )
        {
            msg_debug( coremsg, "already stopped producer named <" << this->get_name() << ">" << eom );
            return true;
        }

        throw error() << "producer named <" << this->get_name() << "> cannot stop from state <" << f_state << ">";
        return false;
    }
    template< class x_type, class x_out_list >
    inline void _producer< x_type, x_out_list >::finalize()
    {
        if( f_state == e_initialized )
        {
            msg_debug( coremsg, "finalizing producer named <" << this->get_name() << ">" << eom );
            f_state = e_idle;

            msg_debug( coremsg, "  finalizing self" << eom );
            finalize_producer();

            msg_debug( coremsg, "finalized producer named <" << this->get_name() << ">" << eom );
            return;
        }

        if( f_state == e_idle )
        {
            msg_debug( coremsg, "already finalized producer named <" << this->get_name() << ">" << eom );
            return;
        }

        throw error() << "producer named <" << this->get_name() << "> cannot finalize from state <" << f_state << ">";
        return;
    }

    template< class x_type, class x_out_list >
    inline void _producer< x_type, x_out_list >::initialize_producer()
    {
        return;
    }
    template< class x_type, class x_out_list >
    inline bool _producer< x_type, x_out_list >::start_producer()
    {
        return true;
    }
    template< class x_type, class x_out_list >
    inline bool _producer< x_type, x_out_list >::execute_producer()
    {
        return true;
    }
    template< class x_type, class x_out_list >
    inline bool _producer< x_type, x_out_list >::stop_producer()
    {
        return true;
    }
    template< class x_type, class x_out_list >
    inline void _producer< x_type, x_out_list >::finalize_producer()
    {
        return;
    }

}

#endif
