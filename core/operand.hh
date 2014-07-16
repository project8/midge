#ifndef _midge_operand_hh_
#define _midge_operand_hh_

#include "component.hh"

#include <vector>
using std::vector;

namespace midge
{

    template< class x_type, class x_data >
    class operand :
        public component
    {
        public:
            operand();
            virtual ~operand();

            //****
            //data
            //****

        public:
            void set_size( const uint64_t& p_size );
            const uint64_t& get_size() const;

            x_data* data();
            const x_data* data() const;

        protected:
            uint64_t f_size;
            x_data* f_data;

            //***********
            //composition
            //***********

        public:
            void set_parent( node* p_parent );
            void add_child( node* p_child );

        protected:
            node* f_parent;
            vector< node* > f_children;

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

            virtual void initialize_operand();
            virtual void deinitialize_operand();
            virtual void update_operand();
            virtual void deupdate_operand();
    };

    template< class x_type, class x_data >
    inline operand< x_type, x_data >::operand() :
            f_size( 0 ),
            f_data( NULL ),
            f_parent( NULL ),
            f_children(),
            f_state( e_idle )
    {
        input( this, &operand< x_type, x_data >::set_parent, "set_parent" );
        output( this, &operand< x_type, x_data >::add_child, "add_child" );
    }
    template< class x_type, class x_data >
    inline operand< x_type, x_data >::~operand()
    {
    }

    //****
    //data
    //****

    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::set_size( const uint64_t& p_size )
    {
        f_size = p_size;
        return;
    }
    template< class x_type, class x_data >
    inline const uint64_t& operand< x_type, x_data >::get_size() const
    {
        return f_size;
    }

    template< class x_type, class x_data >
    inline x_data* operand< x_type, x_data >::data()
    {
        return f_data;
    }

    //***********
    //composition
    //***********

    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::set_parent( node* p_parent )
    {
        f_parent = p_parent;
        return;
    }

    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::add_child( node* p_child )
    {
        f_children.push_back( p_child );
        return;
    }

    //******
    //action
    //******

    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::initialize( node* p_node )
    {
        if( p_node != f_parent )
        {
            //todo: throw here
        }

        if( f_state == e_idle )
        {
            f_state = e_initialized;

            f_data = new x_data[ f_size ];

            initialize_operand();

            for( typename vector< node* >::iterator t_it = f_children.begin(); t_it != f_children.end(); t_it++ )
            {
                (*t_it)->initialize( this );
            }
        }

        if( f_state != e_initialized )
        {
            //todo: throw here
        }

        return;
    }
    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::deinitialize( node* p_node )
    {
        if( p_node != f_parent )
        {
            //todo: throw here
        }

        if( f_state == e_initialized )
        {
            f_state = e_idle;

            deinitialize_operand();

            delete[] f_data;

            for( typename vector< node* >::iterator t_it = f_children.begin(); t_it != f_children.end(); t_it++ )
            {
                (*t_it)->deinitialize( this );
            }
        }

        if( f_state != e_idle )
        {
            //todo: throw here
        }

        return;
    }
    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::update( node* p_node )
    {
        if( p_node != f_parent )
        {
            //todo: throw here
        }

        if( f_state == e_initialized )
        {
            f_state = e_updated;

            update_operand();

            for( typename vector< node* >::iterator t_it = f_children.begin(); t_it != f_children.end(); t_it++ )
            {
                (*t_it)->update( this );
            }
        }

        if( f_state != e_updated )
        {
            //todo: throw here
        }

        return;
    }
    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::deupdate( node* p_node )
    {
        if( p_node != f_parent )
        {
            //todo: throw here
        }

        if( f_state == e_updated )
        {
            f_state = e_initialized;

            deupdate_operand();

            for( typename vector< node* >::iterator t_it = f_children.begin(); t_it != f_children.end(); t_it++ )
            {
                (*t_it)->deupdate( this );
            }
        }

        if( f_state != e_initialized )
        {
            //todo: throw here
        }

        return;
    }

    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::initialize_operand()
    {
        return;
    }
    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::deinitialize_operand()
    {
        return;
    }
    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::update_operand()
    {
        return;
    }
    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::deupdate_operand()
    {
        return;
    }

}

#endif
