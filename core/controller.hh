#ifndef _midge_controller_hh_
#define _midge_controller_hh_

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
            count_t f_count;
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
            f_count( 0 ),
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
            f_state = e_initialized;

            initialize_controller();
        }

        if( f_state != e_initialized )
        {
            throw error() << "controller named <" << this->get_name() << "> cannot initialize from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _controller< x_type, x_in_list, x_out_list >::start()
    {
        if( f_state == e_initialized )
        {
            f_state = e_starting;

            for( count_t t_index = 0; t_index < typelength< x_in_list >::result; t_index++ )
            {
                if( f_ins[ t_index ] != NULL )
                {
                    f_set_ins[ f_set_in_size ] = f_ins[ t_index ];
                    f_set_in_size++;
                }
            }

            for( count_t t_index = 0; t_index < typelength< x_out_list >::result; t_index++ )
            {
                if( f_outs[ t_index ] == NULL )
                {
                    f_set_outs[ f_set_out_size ] = f_outs[ t_index ];
                    f_set_out_size++;
                }
            }

            if( start_controller_pre() == false )
            {
                return false;
            }

            f_count = 0;
            for( count_t t_index = 0; t_index < f_set_out_size; t_index++ )
            {
                if( f_set_outs[ t_index ]->start() == false )
                {
                    return false;
                }
            }
            if( f_count != f_set_in_size )
            {
                return false;
            }

            if( start_controller_post() == false )
            {
                return false;
            }

            f_state = e_started;

            return true;
        }
        else if( f_state == e_starting )
        {
            f_count++;
            return true;
        }
        else if( f_state != e_started )
        {
            throw error() << "controller named <" << this->get_name() << "> cannot start from state <" << f_state << ">";
            return false;
        }

        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _controller< x_type, x_in_list, x_out_list >::execute()
    {
        if( f_state == e_started )
        {
            f_state = e_executing;

            if( execute_controller_pre() == false )
            {
                return false;
            }

            f_count = 0;
            for( count_t t_index = 0; t_index < f_set_out_size; t_index++ )
            {
                if( f_set_outs[ t_index ]->execute() == false )
                {
                    return false;
                }
            }
            if( f_count != f_set_in_size )
            {
                return false;
            }

            if( start_controller_post() == false )
            {
                return false;
            }

            f_state = e_started;

            return true;
        }
        else if( f_state == e_executing )
        {
            f_count++;
            return true;
        }
        else
        {
            throw error() << "controller named <" << this->get_name() << "> cannot execute from state <" << f_state << ">";
            return false;
        }

        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _controller< x_type, x_in_list, x_out_list >::stop()
    {
        if( f_state == e_started )
        {
            f_state = e_stopping;

            if( stop_controller_pre() == false )
            {
                return false;
            }

            f_count = 0;
            for( count_t t_index = 0; t_index < f_set_out_size; t_index++ )
            {
                if( f_set_outs[ t_index ]->stop() == false )
                {
                    return false;
                }
            }
            if( f_count != f_set_in_size )
            {
                return false;
            }

            if( stop_controller_post() == false )
            {
                return false;
            }

            f_state = e_initialized;

            return true;
        }
        else if( f_state == e_stopping )
        {
            f_count++;
            return true;
        }
        else if( f_state != e_initialized )
        {
            throw error() << "controller named <" << this->get_name() << "> cannot stop from state <" << f_state << ">";
            return false;
        }

        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline void _controller< x_type, x_in_list, x_out_list >::finalize()
    {
        if( f_state == e_initialized )
        {
            f_state = e_idle;

            finalize_controller();
        }

        if( f_state != e_idle )
        {
            throw error() << "controller named <" << this->get_name() << "> cannot finalize from state <" << f_state << ">";
        }

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
