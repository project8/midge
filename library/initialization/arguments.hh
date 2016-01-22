#ifndef _midge_arguments_hh_
#define _midge_arguments_hh_

#include <string>

#include "midge_error.hh"

#include <sstream>
#include <set>
#include <map>
#include <utility>

namespace midge
{

    class arguments
    {
        public:
            arguments();
            ~arguments();

        public:
            void required( const std::string& p_key );
            void optional( const std::string& p_key, const std::string& p_default );
            void start( int p_count, char** p_values );
            void stop();

            std::string value() const;

            template< class x_type >
            x_type value( const std::string& p_key ) const;

        private:
            std::string f_value;
            std::set< std::string > f_required_values;
            std::map< std::string, std::string > f_optional_values;
            mutable std::map< std::string, std::string > f_unused_values;
            mutable std::map< std::string, std::string > f_used_values;
    };

    inline std::string arguments::value() const
    {
        return f_value;
    }

    template< class x_type >
    inline x_type arguments::value( const std::string& p_key ) const
    {
        x_type t_value;

        std::map< std::string, std::string >::const_iterator t_it;

        t_it = f_used_values.find( p_key );
        if( t_it != f_used_values.end() )
        {
            std::stringstream t_converter;
            t_converter << t_it->second;
            t_converter >> t_value;
            return t_value;
        }

        t_it = f_unused_values.find( p_key );
        if( t_it != f_unused_values.end() )
        {
            std::stringstream t_converter;
            t_converter << t_it->second;
            t_converter >> t_value;
            f_used_values.insert( std::pair< std::string, std::string >( t_it->first, t_it->second ) );
            f_unused_values.erase( t_it );
            return t_value;
        }

        throw error() << "requested argument <" << p_key << "> not found";
        std::stringstream t_converter;
        t_converter << t_it->second;
        t_converter >> t_value;
        return t_value;
    }

}

#endif
