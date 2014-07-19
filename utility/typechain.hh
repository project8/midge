#ifndef _midge_typechain_hh_
#define _midge_typechain_hh_

#include "typenode.hh"
#include "typenull.hh"

namespace midge
{

    template< class x_node, template< class, int > class x_socket, int x_index = 0 >
    class typechain;

    template< class x_head, class x_tail, template< class, int > class x_socket, int x_index >
    class typechain< typenode< x_head, x_tail >, x_socket, x_index > :
        public x_socket< x_head, x_index >,
        public typechain< x_tail, x_socket, x_index + 1 >
    {
        public:
            typechain() :
                    x_socket< x_head, x_index >(),
                    typechain< x_tail, x_socket, x_index + 1 >()
            {
            }

            template< class x_1 >
            typechain( x_1& p_1 ) :
                    x_socket< x_head, x_index >( p_1 ),
                    typechain< x_tail, x_socket, x_index + 1 >( p_1 )
            {
            }

            template< class x_1, class x_2 >
            typechain( x_1& p_1, x_2& p_2 ) :
                    x_socket< x_head, x_index >( p_1, p_2 ),
                    typechain< x_tail, x_socket, x_index + 1 >( p_1, p_2 )
            {
            }

            template< class x_1, class x_2, class x_3 >
            typechain( x_1& p_1, x_2& p_2, x_3& p_3 ) :
                    x_socket< x_head, x_index >( p_1, p_2, p_3 ),
                    typechain< x_tail, x_socket, x_index + 1 >( p_1, p_2, p_3 )
            {
            }

            template< class x_1, class x_2, class x_3, class x_4 >
            typechain( x_1& p_1, x_2& p_2, x_3& p_3, x_4& p_4 ) :
                    x_socket< x_head, x_index >( p_1, p_2, p_3, p_4 ),
                    typechain< x_tail, x_socket, x_index + 1 >( p_1, p_2, p_3, p_4 )
            {
            }

            template< class x_1, class x_2, class x_3, class x_4, class x_5 >
            typechain( x_1& p_1, x_2& p_2, x_3& p_3, x_4& p_4, x_5& p_5 ) :
                    x_socket< x_head, x_index >( p_1, p_2, p_3, p_4, p_5 ),
                    typechain< x_tail, x_socket, x_index + 1 >( p_1, p_2, p_3, p_4, p_5 )
            {
            }

            virtual ~typechain()
            {
            }
    };

    template< template< class, int > class x_socket, int x_index >
    class typechain< typenull, x_socket, x_index >
    {
        public:
            typechain()
            {
            }

            template< class x_1 >
            typechain( x_1& )
            {
            }

            template< class x_1, class x_2 >
            typechain( x_1&, x_2& )
            {
            }

            template< class x_1, class x_2, class x_3 >
            typechain( x_1&, x_2&, x_3& )
            {
            }

            template< class x_1, class x_2, class x_3, class x_4 >
            typechain( x_1&, x_2&, x_3&, x_4& )
            {
            }

            template< class x_1, class x_2, class x_3, class x_4, class x_5 >
            typechain( x_1&, x_2&, x_3&, x_4&, x_5& )
            {
            }

            virtual ~typechain()
            {
            }
    };

}

#endif
