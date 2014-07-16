#ifndef _midge_member_hh_
#define _midge_member_hh_

#include "link.hh"

namespace midge
{

    template< class x_parent, class x_member, class x_child >
    class member :
        public link
    {
        public:
            member( x_parent p_parent, x_member p_member );
            virtual ~member();

        public:
            virtual void connect( node* p_node );

        protected:
            x_parent f_parent;
            x_member f_member;
            x_child f_child;
    };

    template< class x_parent, class x_member, class x_child >
    member< x_parent, x_member, x_child >::member( x_parent p_parent, x_member p_member ) :
            f_parent( p_parent ),
            f_member( p_member ),
            f_child( NULL )
    {
    }
    template< class x_parent, class x_member, class x_child >
    member< x_parent, x_member, x_child >::~member()
    {
    }

    template< class x_parent, class x_member, class x_child >
    void member< x_parent, x_member, x_child >::connect( node* p_node )
    {
        if( f_child != NULL )
        {
            //todo: throw here
        }

        f_child = dynamic_cast< x_child >( p_node );

        if( f_child == NULL )
        {
            //todo: throw here
        }

        (f_parent->*f_member)( f_child );

        return;
    }

}

#endif
