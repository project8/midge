#ifndef _midge_transformer_hh_
#define _midge_transformer_hh_

#include "_in.hh"
#include "_out.hh"
#include "node.hh"
#include "typeat.hh"
#include "typechain.hh"

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

    template< class x_in_list, class x_out_list >
    class _transformer;

    template< template<class...> class x_in_list, template<class...> class x_out_list, class... x_in_types, class... x_out_types >
    class _transformer< x_in_list<x_in_types...>, x_out_list<x_out_types...> > :
        public transformer,
        public type_start_chain< _in< _type, _index >, x_in_types... >,
        public type_start_chain< _out< _type, _index >, x_out_types... >
    {
        public:
            using node::in;
            using node::out;

        public:
            _transformer();
            virtual ~_transformer();

        protected:
            template< int x_index >
            _stream< type_at< x_index, x_in_types... > >& in_stream()
            {
                return this->_in< type_at< x_index, x_in_types... >, type_int< x_index > >::get_stream();
            }

            template< int x_index >
            _stream< type_at< x_index, x_out_types... > >& out_stream()
            {
                return this->_out< type_at< x_index, x_out_types... >, type_int< x_index > >::get_stream();
            }

            template< int x_index >
            _buffer< type_at< x_index, x_out_types... > >& out_buffer()
            {
                return this->_out< type_at< x_index, x_out_types... >, type_int< x_index > >::get_buffer();
            }
    };

    template< template<class...> class x_in_list, template<class...> class x_out_list, class... x_in_types, class... x_out_types >
    _transformer< x_in_list<x_in_types...>, x_out_list<x_out_types...> >::_transformer() :
            node(),
            type_start_chain< _in< _type, _index >, x_in_types... >(),
            type_start_chain< _out< _type, _index >, x_out_types... >()
    {
    }
    template< template<class...> class x_in_list, template<class...> class x_out_list, class... x_in_types, class... x_out_types >
    _transformer< x_in_list<x_in_types...>, x_out_list<x_out_types...> >::~_transformer()
    {
    }

}

#endif
