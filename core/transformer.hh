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

    template< class x_type, class x_input_list, class x_output_list >
    class _transformer :
        public transformer,
        public typechain< x_input_list, _in >,
        public typechain< x_output_list, _out >
    {
        public:
            using node::input;
            using node::output;

        public:
            _transformer();
            virtual ~_transformer();

            //***********
            //composition
            //***********

        protected:
            template< int x_index >
            typename typeat< x_input_list, x_index >::result* input()
            {
                return this->_in< typename typeat< x_input_list, x_index >::result, x_index >::get();
            }

            template< int x_index >
            typename typeat< x_output_list, x_index >::result* output()
            {
                return this->_out< typename typeat< x_output_list, x_index >::result, x_index >::get();
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
            node* f_outputs[ typelength< x_output_list >::result ];

            virtual void initialize_transformer();
            virtual void execute_transformer();
            virtual void finalize_transformer();
    };

    template< class x_type, class x_input_list, class x_output_list >
    _transformer< x_type, x_input_list, x_output_list >::_transformer() :
            node(),
            typechain< x_input_list, _in >( f_inputs ),
            typechain< x_output_list, _out >( f_outputs ),
            f_state( e_idle ),
            f_count( 0 )
    {
        for( uint64_t t_index = 0; t_index < typelength< x_input_list >::result; t_index++ )
        {
            f_inputs[ t_index ] = NULL;
        }

        for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
        {
            f_outputs[ t_index ] = NULL;
        }
    }
    template< class x_type, class x_input_list, class x_output_list >
    _transformer< x_type, x_input_list, x_output_list >::~_transformer()
    {
    }

    //******
    //action
    //******

    template< class x_type, class x_input_list, class x_output_list >
    inline void _transformer< x_type, x_input_list, x_output_list >::initialize()
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
                    throw error() << "transformer named <" << this->get_name() << "> cannot initialize with input <" << t_index << "> unset";
                }
            }

            for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
            {
                if( f_outputs[ t_index ] == NULL )
                {
                    throw error() << "transformer named <" << this->get_name() << "> cannot initialize with output <" << t_index << "> unset";
                }
            }

            initialize_transformer();

            for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
            {
                f_outputs[ t_index ]->initialize();
            }
        }

        if( f_state != e_initialized )
        {
            throw error() << "transformer named <" << this->get_name() << "> cannot initialize from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_input_list, class x_output_list >
    inline void _transformer< x_type, x_input_list, x_output_list >::execute()
    {
        if( f_state == e_initialized )
        {
            if( ++f_count != typelength< x_input_list >::result )
            {
                return;
            }

            f_count = 0;

            execute_transformer();

            for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
            {
                f_outputs[ t_index ]->execute();
            }
        }
        else
        {
            throw error() << "transformer named <" << this->get_name() << "> cannot execute from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_input_list, class x_output_list >
    inline void _transformer< x_type, x_input_list, x_output_list >::finalize()
    {
        if( f_state == e_initialized )
        {
            if( ++f_count != typelength< x_input_list >::result )
            {
                return;
            }

            f_count = 0;
            f_state = e_idle;

            finalize_transformer();

            for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
            {
                f_outputs[ t_index ]->finalize();
            }
        }

        if( f_state != e_idle )
        {
            throw error() << "transformer named <" << this->get_name() << "> cannot finalize from state <" << f_state << ">";
        }

        return;
    }

    template< class x_type, class x_input_list, class x_output_list >
    inline void _transformer< x_type, x_input_list, x_output_list >::initialize_transformer()
    {
        return;
    }
    template< class x_type, class x_input_list, class x_output_list >
    inline void _transformer< x_type, x_input_list, x_output_list >::execute_transformer()
    {
        return;
    }
    template< class x_type, class x_input_list, class x_output_list >
    inline void _transformer< x_type, x_input_list, x_output_list >::finalize_transformer()
    {
        return;
    }

}

#endif
