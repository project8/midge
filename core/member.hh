#ifndef _midge_member_hh_
#define _midge_member_hh_

#include "link.hh"

namespace midge
{

    template< class x_target, class x_argument >
    class member :
        public link
    {
        public:
            member( void (x_target::*p_member)( x_argument* ) );
            virtual ~member();

        public:
            member* clone() const;
            void set_target( node* p_node );
            void set_argument( node* p_node );
            void connect();

        protected:
            void (x_target::*f_member)( x_argument* );
            x_target* f_target;
            x_argument* f_argument;
    };

    template< class x_target, class x_argument >
    member< x_target, x_argument >::member( void (x_target::*p_member)( x_argument* ) ) :
            f_member( p_member ),
            f_target( NULL ),
            f_argument( NULL )
    {
    }
    template< class x_target, class x_argument >
    member< x_target, x_argument >::~member()
    {
    }

    template< class x_target, class x_argument >
    member< x_target, x_argument >* member< x_target, x_argument >::clone() const
    {
        member< x_target, x_argument >* t_clone = new member< x_target, x_argument >( f_member );

        t_clone->f_target = f_target;
        t_clone->f_argument = f_argument;

        return t_clone;
    }

    template< class x_target, class x_argument >
    void member< x_target, x_argument >::set_target( node* p_target )
    {
        f_target = dynamic_cast< x_target* >( p_target );

        if( f_target == NULL )
        {
            throw error() << "member named <" << this->get_name() << "> could not cast node <" << p_target->get_name() << "> to target type <" << typeid(x_target).name() << ">";
        }

        return;
    }

    template< class x_target, class x_argument >
    void member< x_target, x_argument >::set_argument( node* p_argument )
    {
        f_argument = dynamic_cast< x_argument* >( p_argument );

        if( f_argument == NULL )
        {
            throw error() << "member named <" << this->get_name() << "> could not cast node <" << p_argument->get_name() << "> to argument type <" << typeid(x_argument).name() << ">";
        }

        return;
    }

    template< class x_target, class x_argument >
    void member< x_target, x_argument >::connect()
    {
        if( f_target == NULL )
        {
            throw error() << "member named <" << this->get_name() << "> cannot connect with target unset";
        }

        if( f_argument == NULL )
        {
            throw error() << "member named <" << this->get_name() << "> cannot connect with argument unset";
        }

        (f_target->*f_member)( f_argument );

        return;
    }

}

#endif
