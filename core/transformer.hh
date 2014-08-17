#ifndef _midge_transformer_hh_
#define _midge_transformer_hh_

#include "coremsg.hh"

#include "node.hh"
#include "typechain.hh"
#include "typelength.hh"
#include "typeat.hh"
#include "_in.hh"
#include "_out.hh"

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

    template< class x_type, class x_in_list, class x_out_list >
    class _transformer :
        public transformer,
        public typechain< _in, x_in_list >,
        public typechain< _out, x_out_list >
    {
        public:
            using node::in;
            using node::out;

        public:
            _transformer();
            virtual ~_transformer();

        protected:
            template< int x_index >
            const _stream< typename typeat< x_in_list, x_index >::result >& in_stream()
            {
                return this->_in< typename typeat< x_in_list, x_index >::result, x_index >::get_stream();
            }

            template< int x_index >
            _stream< typename typeat< x_out_list, x_index >::result >& out_stream()
            {
                return this->_out< typename typeat< x_out_list, x_index >::result, x_index >::get_stream();
            }

            template< int x_index >
            _buffer< typename typeat< x_out_list, x_index >::result >& out_buffer()
            {
                return this->_out< typename typeat< x_out_list, x_index >::result, x_index >::get_buffer();
            }
    };

    template< class x_type, class x_in_list, class x_out_list >
    _transformer< x_type, x_in_list, x_out_list >::_transformer() :
            node(),
            typechain< _in, x_in_list >(),
            typechain< _out, x_out_list >()
    {
    }
    template< class x_type, class x_in_list, class x_out_list >
    _transformer< x_type, x_in_list, x_out_list >::~_transformer()
    {
    }

}

#endif
