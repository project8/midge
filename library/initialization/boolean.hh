#ifndef _midge_boolean_hh_
#define _midge_boolean_hh_

#include "value.hh"

namespace midge
{

    class boolean :
        public value
    {
        public:
            boolean( const std::string& p_string );
            virtual ~boolean();
            boolean* clone() const;

        public:
            std::string& str();
            const std::string& str() const;

            bool& get();
            const bool& get() const;

        private:
            std::string f_string;
            bool f_value;
    };
}

#endif
