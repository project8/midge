#ifndef _midge_transformer_hh_
#define _midge_transformer_hh_

#include "core_message.hh"

#include "node.hh"
#include "in.hh"
#include "out.hh"
#include "typechain.hh"
#include "typelength.hh"
#include "typeat.hh"

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

namespace midge
{

    class transformer :
        virtual public node
    {
        protected:
            transformer();

        public:
            virtual ~transformer();
    };

    template< class x_type, class x_in_list, class x_out_list >
    class _transformer :
        public transformer,
        public typechain< x_in_list, _in >,
        public typechain< x_out_list, _out >
    {
        public:
            using node::in;
            using node::out;

        public:
            _transformer();
            virtual ~_transformer();

            //***********
            //composition
            //***********

        protected:
            template< int x_index >
            const typename typeat< x_in_list, x_index >::result* in()
            {
                return this->_in< typename typeat< x_in_list, x_index >::result, x_index >::get();
            }

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
            count_t f_calls;
            node* f_ins[ typelength< x_in_list >::result ];
            node* f_outs[ typelength< x_out_list >::result ];

            virtual void initialize_transformer();
            virtual bool start_transformer();
            virtual bool execute_transformer();
            virtual bool stop_transformer();
            virtual void finalize_transformer();
    };

    template< class x_type, class x_in_list, class x_out_list >
    _transformer< x_type, x_in_list, x_out_list >::_transformer() :
            node(),
            typechain< x_in_list, _in >( f_ins ),
            typechain< x_out_list, _out >( f_outs ),
            f_state( e_idle ),
            f_calls( 0 )
    {
        for( count_t t_index = 0; t_index < typelength< x_in_list >::result; t_index++ )
        {
            f_ins[ t_index ] = NULL;
            f_outs[ t_index ] = NULL;
        }
    }
    template< class x_type, class x_in_list, class x_out_list >
    _transformer< x_type, x_in_list, x_out_list >::~_transformer()
    {
    }

    //******
    //action
    //******

    template< class x_type, class x_in_list, class x_out_list >
    inline void _transformer< x_type, x_in_list, x_out_list >::initialize()
    {
        if( f_state == e_idle )
        {
            msg_debug( coremsg, "initializing transformer named <" << this->get_name() << ">" << eom );
            f_state = e_initialized;

            msg_debug( coremsg, "  initializing self" << eom );
            initialize_transformer();

            msg_debug( coremsg, "initialized transformer named <" << this->get_name() << ">" << eom );
            return;
        }

        if( f_state == e_initialized )
        {
            msg_debug( coremsg, "already initialized transformer named <" << this->get_name() << ">" << eom );
            return;
        }

        throw error() << "cannot initialize transformer named <" << this->get_name() << "> from state <" << f_state << ">";
        return;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _transformer< x_type, x_in_list, x_out_list >::start()
    {
        if( f_state == e_initialized )
        {
            if( ++f_calls != typelength< x_in_list >::result )
            {
                msg_debug( coremsg, "starting transformer named <" << this->get_name() << "> on call <" << f_calls << "> of <" << (count_t) (typelength< x_in_list >::result) << ">" << eom );
                return true;
            }
            f_calls = 0;

            msg_debug( coremsg, "starting transformer named <" << this->get_name() << ">" << eom );
            f_state = e_started;

            msg_debug( coremsg, "  checking ins" << eom );
            for( count_t t_index = 0; t_index < typelength< x_in_list >::result; t_index++ )
            {
                if( f_ins[ t_index ] == NULL )
                {
                    throw error() << "transformer named <" << this->get_name() << "> cannot start with in <" << t_index << "> unset";
                    return false;
                }
            }

            msg_debug( coremsg, "  checking outs" << eom );
            for( count_t t_index = 0; t_index < typelength< x_out_list >::result; t_index++ )
            {
                if( f_outs[ t_index ] == NULL )
                {
                    throw error() << "transformer named <" << this->get_name() << "> cannot start with out <" << t_index << "> unset";
                    return false;
                }
            }

            msg_debug( coremsg, "  starting self" << eom );
            if( start_transformer() == false )
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

            msg_debug( coremsg, "started transformer named <" << this->get_name() << ">" << eom );
            return true;
        }

        if( f_state == e_started )
        {
            msg_debug( coremsg, "already started transformer named <" << this->get_name() << ">" << eom );
            return true;
        }

        throw error() << "transformer named <" << this->get_name() << "> cannot start from state <" << f_state << ">";
        return false;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _transformer< x_type, x_in_list, x_out_list >::execute()
    {
        if( f_state == e_started )
        {
            if( ++f_calls != typelength< x_in_list >::result )
            {
                msg_debug( coremsg, "executing transformer named <" << this->get_name() << "> on call <" << f_calls << "> of <" << (count_t) (typelength< x_in_list >::result) << ">" << eom );
                return true;
            }
            f_calls = 0;

            msg_debug( coremsg, "executing transformer named <" << this->get_name() << ">" << eom );

            msg_debug( coremsg, "  executing self" << eom );
            if( execute_transformer() == false )
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

            msg_debug( coremsg, "executed transformer named <" << this->get_name() << ">" << eom );
            return true;
        }

        throw error() << "transformer named <" << this->get_name() << "> cannot execute from state <" << f_state << ">";
        return false;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _transformer< x_type, x_in_list, x_out_list >::stop()
    {
        if( f_state == e_started )
        {
            if( ++f_calls != typelength< x_in_list >::result )
            {
                msg_debug( coremsg, "stopping transformer named <" << this->get_name() << "> on call <" << f_calls << "> of <" << (count_t) (typelength< x_in_list >::result) << ">" << eom );
                return true;
            }
            f_calls = 0;

            msg_debug( coremsg, "stopping transformer named <" << this->get_name() << ">" << eom );
            f_state = e_initialized;

            msg_debug( coremsg, "  stopping self" << eom );
            if( stop_transformer() == false )
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

            msg_debug( coremsg, "stopped transformer named <" << this->get_name() << ">" << eom );
            return true;
        }

        if( f_state == e_initialized )
        {
            msg_debug( coremsg, "already stopped transformer named <" << this->get_name() << ">" << eom );
            return true;
        }

        throw error() << "transformer named <" << this->get_name() << "> cannot stop from state <" << f_state << ">";
        return false;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline void _transformer< x_type, x_in_list, x_out_list >::finalize()
    {
        if( f_state == e_initialized )
        {
            msg_debug( coremsg, "finalizing transformer named <" << this->get_name() << ">" << eom );
            f_state = e_idle;

            msg_debug( coremsg, "  finalizing self" << eom );
            finalize_transformer();

            msg_debug( coremsg, "finalized transformer named <" << this->get_name() << ">" << eom );
            return;
        }

        if( f_state == e_idle )
        {
            msg_debug( coremsg, "already finalized transformer named <" << this->get_name() << ">" << eom );
            return;
        }

        throw error() << "transformer named <" << this->get_name() << "> cannot finalize from state <" << f_state << ">";
        return;
    }

    template< class x_type, class x_in_list, class x_out_list >
    inline void _transformer< x_type, x_in_list, x_out_list >::initialize_transformer()
    {
        return;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _transformer< x_type, x_in_list, x_out_list >::start_transformer()
    {
        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _transformer< x_type, x_in_list, x_out_list >::execute_transformer()
    {
        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _transformer< x_type, x_in_list, x_out_list >::stop_transformer()
    {
        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline void _transformer< x_type, x_in_list, x_out_list >::finalize_transformer()
    {
        return;
    }

}

#endif
