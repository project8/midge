#ifndef _midge_transformer_hh_
#define _midge_transformer_hh_

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
                e_idle = 0, e_initialized = 1, e_started = 2
            } state;
            state f_state;
            count_t f_count;
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
            f_count( 0 )
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
            f_state = e_initialized;

            initialize_transformer();
        }

        if( f_state != e_initialized )
        {
            throw error() << "transformer named <" << this->get_name() << "> cannot initialize from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _transformer< x_type, x_in_list, x_out_list >::start()
    {
        if( f_state == e_initialized )
        {
            if( ++f_count != typelength< x_in_list >::result )
            {
                return true;
            }
            f_count = 0;

            f_state = e_started;

            for( count_t t_index = 0; t_index < typelength< x_in_list >::result; t_index++ )
            {
                if( f_ins[ t_index ] == NULL )
                {
                    throw error() << "transformer named <" << this->get_name() << "> cannot start with in <" << t_index << "> unset";
                    return false;
                }
            }

            for( count_t t_index = 0; t_index < typelength< x_out_list >::result; t_index++ )
            {
                if( f_outs[ t_index ] == NULL )
                {
                    throw error() << "transformer named <" << this->get_name() << "> cannot start with out <" << t_index << "> unset";
                    return false;
                }
            }

            if( start_transformer() == false )
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
            throw error() << "transformer named <" << this->get_name() << "> cannot start from state <" << f_state << ">";
            return false;
        }

        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _transformer< x_type, x_in_list, x_out_list >::execute()
    {
        if( f_state == e_started )
        {
            if( ++f_count != typelength< x_in_list >::result )
            {
                return true;
            }
            f_count = 0;

            if( execute_transformer() == false )
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
            throw error() << "transformer named <" << this->get_name() << "> cannot execute from state <" << f_state << ">";
            return false;
        }

        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline bool _transformer< x_type, x_in_list, x_out_list >::stop()
    {
        if( f_state == e_started )
        {
            if( ++f_count != typelength< x_in_list >::result )
            {
                return true;
            }
            f_count = 0;

            f_state = e_initialized;

            if( stop_transformer() == false )
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
            throw error() << "transformer named <" << this->get_name() << "> cannot stop from state <" << f_state << ">";
            return false;
        }

        return true;
    }
    template< class x_type, class x_in_list, class x_out_list >
    inline void _transformer< x_type, x_in_list, x_out_list >::finalize()
    {
        if( f_state == e_initialized )
        {
            f_state = e_idle;

            finalize_transformer();
        }

        if( f_state != e_idle )
        {
            throw error() << "transformer named <" << this->get_name() << "> cannot finalize from state <" << f_state << ">";
        }

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
