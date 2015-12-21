#ifndef _midge_object_hh_
#define _midge_object_hh_

#include <utility>

#include "value.hh"
using std::pair;

#include <map>
using std::multimap;

#include <vector>
using std::vector;

namespace midge
{

    class object :
        public value
    {
        public:
            object();
            virtual ~object();
            object* clone() const;

        public:
            count_t size() const;
            void add( const string& p_key, value* p_value );

            pair< string, value* > at( const count_t& p_index );
            pair< string, const value* > at( const count_t& p_index ) const;

            value* at( const string& p_key, const count_t& p_index = 0 );
            const value* at( const string& p_key, const count_t& p_index = 0 ) const;

        private:
            typedef vector< pair< string, value* > > vector_t;
            typedef vector_t::iterator vector_it_t;
            typedef vector_t::const_iterator vector_cit_t;
            typedef vector_t::value_type vector_entry_t;

            vector_t f_vector;

            typedef multimap< string, value* > multimap_t;
            typedef multimap_t::iterator multimap_it_t;
            typedef multimap_t::const_iterator multimap_cit_t;
            typedef multimap_t::value_type multimap_entry_t;

            multimap_t f_multimap;
    };

}

#endif
