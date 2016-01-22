#ifndef _midge_object_hh_
#define _midge_object_hh_

#include <utility>

#include "value.hh"

#include <map>
#include <vector>

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
            void add( const std::string& p_key, value* p_value );

            std::pair< std::string, value* > at( const count_t& p_index );
            std::pair< std::string, const value* > at( const count_t& p_index ) const;

            value* at( const std::string& p_key, const count_t& p_index = 0 );
            const value* at( const std::string& p_key, const count_t& p_index = 0 ) const;

        private:
            typedef std::vector< std::pair< std::string, value* > > vector_t;
            typedef vector_t::iterator vector_it_t;
            typedef vector_t::const_iterator vector_cit_t;
            typedef vector_t::value_type vector_entry_t;

            vector_t f_vector;

            typedef std::multimap< std::string, value* > multimap_t;
            typedef multimap_t::iterator multimap_it_t;
            typedef multimap_t::const_iterator multimap_cit_t;
            typedef multimap_t::value_type multimap_entry_t;

            multimap_t f_multimap;
    };

}

#endif
