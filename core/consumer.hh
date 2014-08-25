#ifndef _midge_consumer_hh_
#define _midge_consumer_hh_

#include "coremsg.hh"

#include "node.hh"
#include "typechain.hh"
#include "typelength.hh"
#include "typeat.hh"
#include "_in.hh"

#include <vector>
using std::vector;

namespace midge
{

    class consumer :
        virtual public node
    {
        protected:
            consumer();

        public:
            virtual ~consumer();
    };

    template< class x_type, class x_in_list >
    class _consumer :
        public consumer,
        public typechain< _in< _1, _index >, x_in_list >
    {
        public:
            using node::in;

        public:
            _consumer();
            virtual ~_consumer();

        protected:
            template< int x_index >
            _stream< typename typeat< x_in_list, x_index >::result >& in_stream()
            {
                return this->_in< typename typeat< x_in_list, x_index >::result, typeint< x_index > >::get_stream();
            }
    };

    template< class x_type, class x_in_list >
    _consumer< x_type, x_in_list >::_consumer() :
            consumer(),
            typechain< _in< _1, _index >, x_in_list >()
    {
    }
    template< class x_type, class x_in_list >
    _consumer< x_type, x_in_list >::~_consumer()
    {
    }

}

#endif
