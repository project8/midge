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

            double& get();
            const double& get() const;

        private:
            string f_string;
            double f_value;
    };
}

#endif
