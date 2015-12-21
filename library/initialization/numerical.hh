#ifndef _midge_numerical_hh_
#define _midge_numerical_hh_

#include "value.hh"

namespace midge
{

    class numerical :
        public value
    {
        public:
            numerical( const string& p_string );
            virtual ~numerical();
            numerical* clone() const;

        public:
            string& str();
            const string& str() const;

            real_t& get();
            const real_t& get() const;

        private:
            string f_string;
            real_t f_value;
    };
}

#endif
