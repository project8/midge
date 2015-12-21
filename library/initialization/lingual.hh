#ifndef _midge_lingual_hh_
#define _midge_lingual_hh_

#include "../library/initialization/value.hh"

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
            count_t size() const;
            const value* at( const string& p_key ) const;
            const value* at( const count_t& p_index ) const;

        public:
            string& str();
            const string& str() const;

        private:
            string f_string;
    };
}

#endif
