#ifndef _midge_producer_hh_
#define _midge_producer_hh_

#include <vector>

#include "../library/core/_out.hh"
#include "../library/core/coremsg.hh"
#include "../library/core/node.hh"
#include "../utility/typeat.hh"
#include "../utility/typechain.hh"
#include "../utility/typelength.hh"
using std::vector;

namespace midge
{

    class producer :
        virtual public node
    {
        protected:
            producer();

        public:
            virtual ~producer();
    };

    template< class x_type, class x_out_list >
    class _producer :
        public producer,
        public typechain< _out< _1, _index >, x_out_list >
    {
        public:
            using node::out;

        public:
            _producer();
            virtual ~_producer();

        protected:
            template< int x_index >
            _stream< typename typeat< x_out_list, x_index >::result >& out_stream()
            {
                return this->_out< typename typeat< x_out_list, x_index >::result, typeint< x_index > >::get_stream();
            }

            template< int x_index >
            _buffer< typename typeat< x_out_list, x_index >::result >& out_buffer()
            {
                return this->_out< typename typeat< x_out_list, x_index >::result, typeint< x_index > >::get_buffer();
            }
    };

    template< class x_type, class x_out_list >
    _producer< x_type, x_out_list >::_producer() :
            producer(),
            typechain< _out< _1, _index >, x_out_list >()
    {
    }
    template< class x_type, class x_out_list >
    _producer< x_type, x_out_list >::~_producer()
    {
    }

}

#endif
