#ifndef _midge_consumer_hh_
#define _midge_consumer_hh_

#include "_in.hh"
#include "node.hh"
#include "typeat.hh"
#include "typechain.hh"

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
    class _consumer;

    template< class x_type, template<class...> class x_in_list, class... x_in_types >
    class _consumer< x_type, x_in_list<x_in_types...> > :
        public consumer,
        public type_start_chain< _in< _type, _index >, x_in_types... >
    {
        public:
            using node::in;

        public:
            _consumer();
            virtual ~_consumer();

        protected:
            template< int x_index >
            _stream< type_at< x_index, x_in_types... > >& in_stream()
            {
                return this->_in< type_at< x_index, x_in_types... >, type_int< x_index > >::get_stream();
            }
    };

    template< class x_type, template<class...> class x_in_list, class... x_in_types >
    _consumer< x_type, x_in_list<x_in_types...> >::_consumer() :
            consumer(),
            type_start_chain< _in< _type, _index >, x_in_types... >()
    {
    }

    template< class x_type, template<class...> class x_in_list, class... x_in_types >
    _consumer< x_type, x_in_list<x_in_types...> >::~_consumer()
    {
    }

}

#endif
