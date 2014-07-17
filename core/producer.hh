#ifndef _midge_producer_hh_
#define _midge_producer_hh_

#include "out.hh"
#include "typechain.hh"
#include "typelength.hh"
#include "typeat.hh"

#include <vector>
using std::vector;

namespace midge
{

    template< class x_type, class x_output_list >
    class producer :
        public typechain< x_output_list, out >
    {
        public:
            using node::output;

        public:
            producer();
            virtual ~producer();

            //***********
            //composition
            //***********

        protected:
            template< int x_index >
            typename typeat< x_output_list, x_index >::result* output()
            {
                return this->out< typename typeat< x_output_list, x_index >::result, x_index >::get();
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
            node* f_outputs[ typelength< x_output_list >::result ];

            virtual void initialize_producer();
            virtual void execute_producer();
            virtual void finalize_producer();
    };

    template< class x_type, class x_output_list >
    producer< x_type, x_output_list >::producer() :
            component(),
            typechain< x_output_list, out >( f_outputs ),
            f_state( e_idle )
    {
        for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
        {
            f_outputs[ t_index ] = NULL;
        }
    }
    template< class x_type, class x_output_list >
    producer< x_type, x_output_list >::~producer()
    {
    }

    //******
    //action
    //******

    template< class x_type, class x_output_list >
    inline void producer< x_type, x_output_list >::initialize()
    {
        if( f_state == e_idle )
        {
            f_state = e_initialized;

            for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
            {
                if( f_outputs[ t_index ] == NULL )
                {
                    throw error() << "producer named <" << this->get_name() << "> cannot initialize with output <" << t_index << "> unset";
                }
            }

            initialize_producer();

            for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
            {
                f_outputs[ t_index ]->initialize();
            }
        }

        if( f_state != e_initialized )
        {
            throw error() << "producer named <" << this->get_name() << "> cannot initialize from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_output_list >
    inline void producer< x_type, x_output_list >::execute()
    {
        if( f_state == e_initialized )
        {
            execute_producer();

            for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
            {
                f_outputs[ t_index ]->execute();
            }
        }
        else
        {
            throw error() << "producer named <" << this->get_name() << "> cannot execute from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_output_list >
    inline void producer< x_type, x_output_list >::finalize()
    {
        if( f_state == e_initialized )
        {
            f_state = e_idle;

            finalize_producer();

            for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
            {
                f_outputs[ t_index ]->finalize();
            }
        }

        if( f_state != e_idle )
        {
            throw error() << "producer named <" << this->get_name() << "> cannot finalize from state <" << f_state << ">";
        }

        return;
    }

    template< class x_type, class x_output_list >
    inline void producer< x_type, x_output_list >::initialize_producer()
    {
        return;
    }
    template< class x_type, class x_output_list >
    inline void producer< x_type, x_output_list >::execute_producer()
    {
        return;
    }
    template< class x_type, class x_output_list >
    inline void producer< x_type, x_output_list >::finalize_producer()
    {
        return;
    }

}

#endif
