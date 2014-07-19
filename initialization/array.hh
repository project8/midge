#ifndef _midge_array_hh_
#define _midge_array_hh_

#include "value.hh"

#include <vector>
using std::vector;

namespace midge
{

    class array :
        public value
    {
        public:
            array();
            virtual ~array();
            array* clone() const;

        public:
            uint64_t size() const;
            void add( value* p_value );

            value* at( const uint64_t& p_index );
            const value* at( const uint64_t& p_index ) const;

        private:
            typedef vector< value* > vector_t;
            typedef vector_t::iterator vector_it_t;
            typedef vector_t::const_iterator vector_cit_t;
            typedef vector_t::value_type vector_entry_t;

            vector_t f_vector;
    };

}

#endif
