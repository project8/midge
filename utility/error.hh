#ifndef _midge_error_hh_
#define _midge_error_hh_

#include <exception>
using std::exception;

#include <sstream>
using std::ostringstream;

namespace midge
{

    class error :
        public exception
    {
        public:
            error();
            error( const error& p_copy );
            virtual ~error() throw ();

            template< class x_type >
            error& operator<<( const x_type& p_fragment );

            const char* what() const throw ();

        private:
            ostringstream f_message;
    };

    template< class x_type >
    error& error::operator<<( const x_type& p_fragment )
    {
        f_message << p_fragment;
        return (*this);
    }

}

#endif
