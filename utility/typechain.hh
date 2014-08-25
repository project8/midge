#ifndef _midge_typechain_hh_
#define _midge_typechain_hh_

#include "typenull.hh"
#include "typeint.hh"
#include "typenode.hh"
#include "typebind.hh"

namespace midge
{

    class _1
    {
    };

    class _index
    {
    };

    template< class x_socket, class x_list, int x_index = 0 >
    class typechain;

    template< class x_socket, class x_head, class x_tail, int x_index >
    class typechain< x_socket, typenode< x_head, x_tail >, x_index > :
        public typebind< typename typebind< x_socket, _1, x_head >::result, _index, typeint< x_index > >::result,
        public typechain< x_socket, x_tail, x_index + 1 >
    {
        private:
            typedef typename typebind< typename typebind< x_socket, _1, x_head >::result, _index, typeint< x_index > >::result this_type;
            typedef typechain< x_socket, x_tail, x_index + 1 > next_type;

        public:
            typechain() :
                    this_type(),
                    next_type()
            {
            }

            template< class x_1 >
            typechain( x_1& p_1 ) :
                    this_type( p_1 ),
                    next_type( p_1 )
            {
            }

            template< class x_1, class x_2 >
            typechain( x_1& p_1, x_2& p_2 ) :
                    this_type( p_1, p_2 ),
                    next_type( p_1, p_2 )
            {
            }

            template< class x_1, class x_2, class x_3 >
            typechain( x_1& p_1, x_2& p_2, x_3& p_3 ) :
                    this_type( p_1, p_2, p_3 ),
                    next_type( p_1, p_2, p_3 )
            {
            }

            template< class x_1, class x_2, class x_3, class x_4 >
            typechain( x_1& p_1, x_2& p_2, x_3& p_3, x_4& p_4 ) :
                    this_type( p_1, p_2, p_3, p_4 ),
                    next_type( p_1, p_2, p_3, p_4 )
            {
            }

            template< class x_1, class x_2, class x_3, class x_4, class x_5 >
            typechain( x_1& p_1, x_2& p_2, x_3& p_3, x_4& p_4, x_5& p_5 ) :
                    this_type( p_1, p_2, p_3, p_4, p_5 ),
                    next_type( p_1, p_2, p_3, p_4, p_5 )
            {
            }

            virtual ~typechain()
            {
            }
    };

    template< class x_socket, int x_index >
    class typechain< x_socket, _, x_index >
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
