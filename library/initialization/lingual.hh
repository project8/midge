#ifndef _midge_lingual_hh_
#define _midge_lingual_hh_

#include "value.hh"

namespace midge
{

    class lingual :
        public value
    {
        public:
            lingual( const std::string& p_value );
            virtual ~lingual();
            lingual* clone() const;

        public:
            count_t size() const;
            const value* at( const std::string& p_key ) const;
            const value* at( const count_t& p_index ) const;

        public:
            std::string& str();
            const std::string& str() const;

        private:
            std::string f_string;
    };
}

#endif
