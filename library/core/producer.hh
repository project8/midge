#ifndef _midge_producer_hh_
#define _midge_producer_hh_

#include "_out.hh"
#include "node.hh"
#include "typeat.hh"
#include "typechain.hh"

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
    class _producer;

    template< class x_type, template<class...> class x_out_list, class... x_out_types >
    class _producer< x_type, x_out_list<x_out_types...> > :
        public producer,
        public type_start_chain< _out< _type, _index >, x_out_types... >
    {
        public:
            using node::out;

        public:
            _producer();
            virtual ~_producer();

        protected:
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

    template< class x_type, template<class...> class x_out_list, class... x_out_types >
    _producer< x_type, x_out_list<x_out_types...> >::_producer() :
            producer(),
            type_start_chain< _out< _type, _index >, x_out_types... >()
    {
    }
    template< class x_type, template<class...> class x_out_list, class... x_out_types >
    _producer< x_type, x_out_list<x_out_types...> >::~_producer()
    {
    }

}

#endif
