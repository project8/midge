#ifndef _midge_boolean_hh_
#define _midge_boolean_hh_

#include "../library/initialization/value.hh"

namespace midge
{

    class boolean :
        public value
    {
        public:
            boolean( const string& p_string );
            virtual ~boolean();
            boolean* clone() const;

        public:
            string& str();
            const string& str() const;

            bool& get();
            const bool& get() const;

        private:
            string f_string;
            bool f_value;
    };
}

#endif
