#ifndef _midge_error_hh_
#define _midge_error_hh_

#include <exception>
#include <string>
#include <sstream>

namespace midge
{

    class error : public std::exception
    {
        public:
            error();
            error( const error& p_copy );
            error& operator=( const error& p_copy );
            virtual ~error() throw ();

            template< class x_type >
            error& operator<<( const x_type& p_fragment );

            const char* what() const throw ();

        private:
            std::string f_message;
    };

    // midge should exit and not be restarted
    class node_fatal_error : public  std::exception
    {
        public:
            node_fatal_error();
            node_fatal_error( const node_fatal_error& p_copy );
            node_fatal_error& operator=( const node_fatal_error& p_copy );
            virtual ~node_fatal_error() throw ();

            template< class x_type >
            node_fatal_error& operator<<( const x_type& p_fragment );

            const char* what() const throw ();

        private:
            std::string f_message;
    };

    // midge should exit but could be restarted
    class node_nonfatal_error : public std::exception
    {
        public:
            node_nonfatal_error();
            node_nonfatal_error( const node_nonfatal_error& p_copy );
            node_nonfatal_error& operator=( const node_nonfatal_error& p_copy );
            virtual ~node_nonfatal_error() throw ();

            template< class x_type >
            node_nonfatal_error& operator<<( const x_type& p_fragment );

            const char* what() const throw ();

        private:
            std::string f_message;
    };

    template< class x_type >
    error& error::operator<<( const x_type& p_fragment )
    {
        std::stringstream f_converter;
        f_converter << f_message << p_fragment;
        f_message.assign( f_converter.str() );
        return (*this);
    }

    template< class x_type >
    node_fatal_error& node_fatal_error::operator<<( const x_type& p_fragment )
    {
        std::stringstream f_converter;
        f_converter << f_message << p_fragment;
        f_message.assign( f_converter.str() );
        return (*this);
    }

    template< class x_type >
    node_nonfatal_error& node_nonfatal_error::operator<<( const x_type& p_fragment )
    {
        std::stringstream f_converter;
        f_converter << f_message << p_fragment;
        f_message.assign( f_converter.str() );
        return (*this);
    }

}

#endif
