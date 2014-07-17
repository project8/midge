#ifndef _midge_token_hh_
#define _midge_token_hh_

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

namespace midge
{

    class token
    {
        public:
            token();
            token( const string& p_value );
            token( const token& p_copy );
            virtual ~token();

            string& str();
            const string& str() const;

            template< class x_type >
            x_type as() const;

        private:
            string f_value;
    };

    template< class x_type >
    x_type token::as() const
    {
        stringstream t_stringstream;
        x_type t_value;
        t_stringstream << f_value;
        t_stringstream >> t_value;
        return t_value;
    }

}

#endif
