#ifndef _midge_lingual_hh_
#define _midge_lingual_hh_

#include "value.hh"

namespace midge
{

    class lingual :
        public value
    {
        public:
            lingual( const string& p_value );
            virtual ~lingual();
            lingual* clone() const;

        public:
            uint64_t size() const;
            const value* at( const string& p_key ) const;
            const value* at( const uint64_t& p_index ) const;

        public:
            string& str();
            const string& str() const;

        private:
            string f_string;
    };
}

#endif
