#ifndef _midge_transformer_hh_
#define _midge_transformer_hh_

#include "in.hh"
#include "out.hh"
#include "typechain.hh"
#include "typelength.hh"
#include "typeat.hh"

#include <vector>
using std::vector;

namespace midge
{

    template< class x_type, class x_input_list, class x_output_list >
    class transformer :
        public typechain< x_input_list, in >,
        public typechain< x_output_list, out >
    {
        public:
            transformer();
            virtual ~transformer();

            //***********
            //composition
            //***********

        protected:
            template< int x_index >
            typename typeat< x_input_list, x_index >::result* input()
            {
                return this->in< typename typeat< x_input_list, x_index >::result, x_index >::get();
            }

            template< int x_index >
            typename typeat< x_output_list, x_index >::result* output()
            {
                return this->out< typename typeat< x_output_list, x_index >::result, x_index >::get();
            }

            //******
            //action
            //******

        public:
            void initialize( node* p_node );
            void deinitialize( node* p_node );
            void update( node* p_node );
            void deupdate( node* p_node );

        protected:
            typedef enum
            {
                e_idle = 0, e_initialized = 1, e_updated = 2
            } state;
            state f_state;
            uint64_t f_count;
            node* f_inputs[ typelength< x_input_list >::result ];
            node* f_outputs[ typelength< x_output_list >::result ];

            virtual void initialize_transformer();
            virtual void deinitialize_transformer();
            virtual void update_transformer();
            virtual void deupdate_transformer();
    };

    template< class x_type, class x_input_list, class x_output_list >
    transformer< x_type, x_input_list, x_output_list >::transformer() :
            component(),
            typechain< x_input_list, in >( f_inputs ),
            typechain< x_output_list, out >( f_outputs ),
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
    transformer< x_type, x_input_list, x_output_list >::~transformer()
    {
    }

    //******
    //action
    //******

    template< class x_type, class x_input_list, class x_output_list >
    inline void transformer< x_type, x_input_list, x_output_list >::initialize( node* p_node )
    {
        if( f_state == e_idle )
        {
            if( ++f_count != typelength< x_input_list >::result )
            {
                return;
            }

            f_state = e_initialized;

            for( uint64_t t_index = 0; t_index < typelength< x_input_list >::result; t_index++ )
            {
                if( f_inputs[ t_index ] == NULL )
                {
                    //todo: throw here
                }
            }

            for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
            {
                if( f_outputs[ t_index ] == NULL )
                {
                    //todo: throw here
                }
            }

            initialize_transformer();

            for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
            {
                f_outputs[ t_index ]->initialize( this );
            }
        }

        if( f_state != e_initialized )
        {
            //todo: throw here
        }

        return;
    }
    template< class x_type, class x_input_list, class x_output_list >
    inline void transformer< x_type, x_input_list, x_output_list >::deinitialize( node* p_node )
    {
        if( f_state == e_initialized )
        {
            if( ++f_count != typelength< x_input_list >::result )
            {
                return;
            }

            f_state = e_idle;

            deinitialize_transformer();

            for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
            {
                f_outputs[ t_index ]->deinitialize( this );
            }
        }

        if( f_state != e_idle )
        {
            //todo: throw here
        }

        return;
    }
    template< class x_type, class x_input_list, class x_output_list >
    inline void transformer< x_type, x_input_list, x_output_list >::update( node* p_node )
    {
        if( f_state == e_initialized )
        {
            if( ++f_count != typelength< x_input_list >::result )
            {
                return;
            }

            f_state = e_updated;

            update_transformer();

            for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
            {
                f_outputs[ t_index ]->update( this );
            }
        }

        if( f_state != e_updated )
        {
            //todo: throw here
        }

        return;
    }
    template< class x_type, class x_input_list, class x_output_list >
    inline void transformer< x_type, x_input_list, x_output_list >::deupdate( node* p_node )
    {
        if( f_state == e_updated )
        {
            if( ++f_count != typelength< x_input_list >::result )
            {
                return;
            }

            f_state = e_initialized;

            deupdate_transformer();

            for( uint64_t t_index = 0; t_index < typelength< x_output_list >::result; t_index++ )
            {
                f_outputs[ t_index ]->deupdate( this );
            }
        }

        if( f_state != e_initialized )
        {
            //todo: throw here
        }

        return;
    }

    template< class x_type, class x_input_list, class x_output_list >
    inline void transformer< x_type, x_input_list, x_output_list >::initialize_transformer()
    {
        return;
    }
    template< class x_type, class x_input_list, class x_output_list >
    inline void transformer< x_type, x_input_list, x_output_list >::deinitialize_transformer()
    {
        return;
    }
    template< class x_type, class x_input_list, class x_output_list >
    inline void transformer< x_type, x_input_list, x_output_list >::update_transformer()
    {
        return;
    }
    template< class x_type, class x_input_list, class x_output_list >
    inline void transformer< x_type, x_input_list, x_output_list >::deupdate_transformer()
    {
        return;
    }

}

#endif
