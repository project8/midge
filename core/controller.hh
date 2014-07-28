#ifndef _midge_controller_hh_
#define _midge_controller_hh_

#include "core_message.hh"

#include "node.hh"
#include "in.hh"
#include "out.hh"
#include "typechain.hh"
#include "typelength.hh"
#include "typeat.hh"

#include <vector>
using std::vector;

namespace midge
{

    class controller :
        virtual public node
    {
        protected:
            controller();

        public:
            virtual ~controller();
    };

    template< class x_type, class x_in_list, class x_out_list >
    class _controller :
        public controller,
        public typechain< x_in_list, _in >,
        public typechain< x_out_list, _out >
    {
        public:
            using node::in;
            using node::out;

        public:
            _controller();
            virtual ~_controller();

            //***********
            //composition
            //***********

        protected:
            template< int x_index >
            typename typeat< x_in_list, x_index >::result* in()
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
                e_idle = 0, e_initialized = 1, e_started = 2, e_starting = 3, e_executing = 4, e_stopping = 5
            } state;
            state f_state;
            count_t f_calls;
            node* f_ins[ typelength< x_in_list >::result ];
            node* f_outs[ typelength< x_out_list >::result ];
            node* f_set_ins[ typelength< x_in_list >::result ];
            count_t f_set_in_size;
            node* f_set_outs[ typelength< x_out_list >::result ];
            count_t f_set_out_size;

            virtual void initialize_controller();
            virtual bool start_controller_pre();
            virtual bool start_controller_post();
            virtual bool execute_controller_pre();
            virtual bool execute_controller_post();
            virtual bool stop_controller_pre();
            virtual bool stop_controller_post();
            virtual void finalize_controller();
    };

    template< class x_type, class x_in_list, class x_out_list >
    _controller< x_type, x_in_list, x_out_list >::_controller() :
            node(),
            typechain< x_in_list, _in >( f_ins ),
            typechain< x_out_list, _out >( f_outs ),
            f_state( e_idle ),
            f_calls( 0 ),
            f_set_in_size( 0 ),
            f_set_out_size( 0 )
    {
        for( count_t t_index = 0; t_index < typelength< x_in_list >::result; t_index++ )
        {
            f_ins[ t_index ] = NULL;
            f_outs[ t_index ] = NULL;
            f_set_ins[ t_index ] = NULL;
            f_set_outs[ t_index ] = NULL;
        }
    }
    template< class x_type, class x_in_list, class x_out_list >
    _controller< x_type, x_in_list, x_out_list >::~_controller()
    {
    }

    //******
    //action
    //******

    template< class x_type, class x_in_list, class x_out_list >
    inline void _controller< x_type, x_in_list, x_out_list >::initialize()
    {
        if( f_state == e_idle )
        {
            msg_debug( coremsg,  "initializing controller <" << this->get_name() << ">" << eom );
            f_state = e_initialized;

            msg_debug( coremsg,  "  initializing self" << eom );
            initialize_controller();

            msg_debug( coremsg,  "initialized controller <" << this->get_name() << ">" << eom );
            return;
        }

        if( f_state == e_initialized )
        {
            msg_debug( coremsg,  "already initialized controller <" << this->get_name() << ">" << eom );
            return;
        }

        throw error() << "controller named <" << this->get_name() << "> cannot initialize from state <" << f_state << ">";
        return;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _controller< x_type, x_in_list, x_out_list >::start()
    {
        if( f_state == e_initialized )
        {
            msg_debug( coremsg,  "starting controller <" << this->get_name() << ">" << eom );
            f_state = e_starting;

            msg_debug( coremsg,  "  checking ins" << eom );
            for( count_t t_index = 0; t_index < typelength< x_in_list >::result; t_index++ )
            {
                if( f_ins[ t_index ] != NULL )
                {
                    msg_debug( coremsg,  "  in at <" << t_index << "> is set" << eom );
                    f_set_ins[ f_set_in_size ] = f_ins[ t_index ];
                    f_set_in_size++;
                }
                else
                {
                    msg_debug( coremsg,  "  in at <" << t_index << "> is null" << eom );
                }
            }

            msg_debug( coremsg,  "  checking outs" << eom );
            for( count_t t_index = 0; t_index < typelength< x_out_list >::result; t_index++ )
            {
                if( f_outs[ t_index ] != NULL )
                {
                    msg_debug( coremsg,  "  out at <" << t_index << "> is set" << eom );
                    f_set_outs[ f_set_out_size ] = f_outs[ t_index ];
                    f_set_out_size++;
                }
                else
                {
                    msg_debug( coremsg,  "  out at <" << t_index << "> is null" << eom );
                }
            }

            msg_debug( coremsg,  "  starting self pre" << eom );
            if( start_controller_pre() == false )
            {
                msg_debug( coremsg,  "    done" << eom );
                f_state = e_initialized;
                return false;
            }

            msg_debug( coremsg,  "  starting outs" << eom );
            f_calls = 0;
            for( count_t t_index = 0; t_index < f_set_out_size; t_index++ )
            {
                msg_debug( coremsg,  "  starting out at <" << t_index << ">" << eom );
                if( f_set_outs[ t_index ]->start() == false )
                {
                    msg_debug( coremsg,  "    done" << eom );
                    f_state = e_initialized;
                    return false;
                }
            }

            msg_debug( coremsg,  "  checking ins" << eom );
            if( f_calls != f_set_in_size )
            {
                msg_debug( coremsg,  "    done" << eom );
                f_state = e_initialized;
                return false;
            }

            msg_debug( coremsg,  "  starting self post" << eom );
            if( start_controller_post() == false )
            {
                msg_debug( coremsg,  "    done" << eom );
                f_state = e_initialized;
                return false;
            }

            msg_debug( coremsg,  "started controller <" << this->get_name() << ">" << eom );
            f_state = e_started;
            return true;
        }

        if( f_state == e_starting )
        {
            msg_debug( coremsg,  "starting controller <" << this->get_name() << "> was called" << eom );
            f_calls++;
            return true;
        }

        if( f_state == e_started )
        {
            return true;
        }

        throw error() << "controller named <" << this->get_name() << "> cannot start from state <" << f_state << ">";
        return false;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _controller< x_type, x_in_list, x_out_list >::execute()
    {
        if( f_state == e_started )
        {
            msg_debug( coremsg,  "executing controller <" << this->get_name() << ">" << eom );
            f_state = e_executing;

            msg_debug( coremsg,  "  executing self pre" << eom );
            if( execute_controller_pre() == false )
            {
                msg_debug( coremsg,  "    done" << eom );
                f_state = e_started;
                return false;
            }

            msg_debug( coremsg,  "  executing outs" << eom );
            f_calls = 0;
            for( count_t t_index = 0; t_index < f_set_out_size; t_index++ )
            {
                msg_debug( coremsg,  "  executing out at <" << t_index << ">" << eom );
                if( f_set_outs[ t_index ]->execute() == false )
                {
                    msg_debug( coremsg,  "    done" << eom );
                    f_state = e_started;
                    return false;
                }
            }

            msg_debug( coremsg,  "  checking ins" << eom );
            if( f_calls != f_set_in_size )
            {
                msg_debug( coremsg,  "    done" << eom );
                f_state = e_started;
                return false;
            }

            msg_debug( coremsg,  "  executing self post" << eom );
            if( execute_controller_post() == false )
            {
                msg_debug( coremsg,  "    done" << eom );
                f_state = e_started;
                return false;
            }

            msg_debug( coremsg,  "executed controller <" << this->get_name() << ">" << eom );
            f_state = e_started;
            return true;
        }

        if( f_state == e_executing )
        {
            msg_debug( coremsg,  "executing controller <" << this->get_name() << "> was called" << eom );
            f_calls++;
            return true;
        }

        throw error() << "controller named <" << this->get_name() << "> cannot execute from state <" << f_state << ">";
        return false;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _controller< x_type, x_in_list, x_out_list >::stop()
    {
        if( f_state == e_started )
        {
            msg_debug( coremsg,  "stopping controller <" << this->get_name() << ">" << eom );
            f_state = e_stopping;

            msg_debug( coremsg,  "  stopping self pre" << eom );
            if( stop_controller_pre() == false )
            {
                msg_debug( coremsg,  "    done" << eom );
                f_state = e_initialized;
                return false;
            }

            msg_debug( coremsg,  "  stopping outs" << eom );
            f_calls = 0;
            for( count_t t_index = 0; t_index < f_set_out_size; t_index++ )
            {
                msg_debug( coremsg,  "  stopping out at <" << t_index << ">" << eom );
                if( f_set_outs[ t_index ]->stop() == false )
                {
                    msg_debug( coremsg,  "    done" << eom );
                    f_state = e_initialized;
                    return false;
                }
            }

            msg_debug( coremsg,  "  checking ins" << eom );
            if( f_calls != f_set_in_size )
            {
                msg_debug( coremsg,  "    done" << eom );
                f_state = e_initialized;
                return false;
            }

            msg_debug( coremsg,  "  stopping self post" << eom );
            if( stop_controller_post() == false )
            {
                msg_debug( coremsg,  "    done" << eom );
                f_state = e_initialized;
                return false;
            }

            msg_debug( coremsg,  "stopped controller <" << this->get_name() << ">" << eom );
            f_state = e_initialized;
            return true;
        }

        if( f_state == e_stopping )
        {
            msg_debug( coremsg,  "stopping controller <" << this->get_name() << "> was called" << eom );
            f_calls++;
            return true;
        }

        if( f_state == e_initialized )
        {
            return true;
        }

        throw error() << "controller named <" << this->get_name() << "> cannot stop from state <" << f_state << ">";
        return false;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline void _controller< x_type, x_in_list, x_out_list >::finalize()
    {
        if( f_state == e_started )
        {
            msg_debug( coremsg,  "finalizing controller <" << this->get_name() << ">" << eom );
            f_state = e_initialized;

            msg_debug( coremsg,  "  finalizing self" << eom );
            finalize_controller();

            msg_debug( coremsg,  "finalized controller <" << this->get_name() << ">" << eom );
            return;
        }

        if( f_state == e_initialized )
        {
            msg_debug( coremsg,  "already finalized controller <" << this->get_name() << ">" << eom );
            return;
        }

        throw error() << "controller named <" << this->get_name() << "> cannot finalize from state <" << f_state << ">";
        return;
    }

    template< class x_type, class x_in_list, class x_out_list >
    inline void _controller< x_type, x_in_list, x_out_list >::initialize_controller()
    {
        return;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _controller< x_type, x_in_list, x_out_list >::start_controller_pre()
    {
        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _controller< x_type, x_in_list, x_out_list >::start_controller_post()
    {
        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _controller< x_type, x_in_list, x_out_list >::execute_controller_pre()
    {
        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _controller< x_type, x_in_list, x_out_list >::execute_controller_post()
    {
        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _controller< x_type, x_in_list, x_out_list >::stop_controller_pre()
    {
        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _controller< x_type, x_in_list, x_out_list >::stop_controller_post()
    {
        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline void _controller< x_type, x_in_list, x_out_list >::finalize_controller()
    {
        return;
    }

}

#endif
