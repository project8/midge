#ifndef _midge_producer_hh_
#define _midge_producer_hh_

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
            f_state = e_initialized;

            initialize_producer();
        }

        if( f_state != e_initialized )
        {
            throw error() << "producer named <" << this->get_name() << "> cannot initialize from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_out_list >
    inline bool _producer< x_type, x_out_list >::start()
    {
        if( f_state == e_initialized )
        {
            f_state = e_started;

            for( count_t t_index = 0; t_index < typelength< x_out_list >::result; t_index++ )
            {
                if( f_outs[ t_index ] == NULL )
                {
                    throw error() << "producer named <" << this->get_name() << "> cannot start with out <" << t_index << "> unset";
                    return false;
                }
            }

            if( start_producer() == false )
            {
                return false;
            }

            for( count_t t_index = 0; t_index < typelength< x_out_list >::result; t_index++ )
            {
                if( f_outs[ t_index ]->start() == false )
                {
                    return false;
                }
            }
        }

        if( f_state != e_started )
        {
            throw error() << "producer named <" << this->get_name() << "> cannot start from state <" << f_state << ">";
            return false;
        }

        return true;
    }
    template< class x_type, class x_out_list >
    inline bool _producer< x_type, x_out_list >::execute()
    {
        if( f_state == e_started )
        {
            if( execute_producer() == false )
            {
                return false;
            }

            for( count_t t_index = 0; t_index < typelength< x_out_list >::result; t_index++ )
            {
                if( f_outs[ t_index ]->execute() == false )
                {
                    return false;
                }
            }
        }
        else
        {
            throw error() << "producer named <" << this->get_name() << "> cannot execute from state <" << f_state << ">";
            return false;
        }

        return true;
    }
    template< class x_type, class x_out_list >
    inline bool _producer< x_type, x_out_list >::stop()
    {
        if( f_state == e_started )
        {
            f_state = e_initialized;

            if( stop_producer() == false )
            {
                return false;
            }

            for( count_t t_index = 0; t_index < typelength< x_out_list >::result; t_index++ )
            {
                if( f_outs[ t_index ]->stop() == false )
                {
                    return false;
                }
            }
        }

        if( f_state != e_initialized )
        {
            throw error() << "producer named <" << this->get_name() << "> cannot stop from state <" << f_state << ">";
            return false;
        }

        return true;
    }
    template< class x_type, class x_out_list >
    inline void _producer< x_type, x_out_list >::finalize()
    {
        if( f_state == e_initialized )
        {
            f_state = e_idle;

            finalize_producer();
        }

        if( f_state != e_idle )
        {
            throw error() << "producer named <" << this->get_name() << "> cannot finalize from state <" << f_state << ">";
        }

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
