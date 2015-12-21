#ifndef _midge_arguments_hh_
#define _midge_arguments_hh_

#include <string>

#include "error.hh"
using std::string;

#include <sstream>
using std::stringstream;

#include <set>
using std::set;

#include <map>
using std::map;

#include <utility>
using std::pair;

namespace midge
{

    class arguments
    {
        public:
            arguments();
            ~arguments();

        public:
            void required( const string& p_key );
            void optional( const string& p_key, const string& p_default );
            void start( int p_count, char** p_values );
            void stop();

            string value() const;

            template< class x_type >
            x_type value( const string& p_key ) const;

        private:
            string f_value;
            set< string > f_required_values;
            map< string, string > f_optional_values;
            mutable map< string, string > f_unused_values;
            mutable map< string, string > f_used_values;
    };

    inline string arguments::value() const
    {
        return f_value;
    }

    template< class x_type >
    inline x_type arguments::value( const string& p_key ) const
    {
        x_type t_value;

        map< string, string >::const_iterator t_it;

        t_it = f_used_values.find( p_key );
        if( t_it != f_used_values.end() )
        {
            stringstream t_converter;
            t_converter << t_it->second;
            t_converter >> t_value;
            return t_value;
        }

        t_it = f_unused_values.find( p_key );
        if( t_it != f_unused_values.end() )
        {
            stringstream t_converter;
            t_converter << t_it->second;
            t_converter >> t_value;
            f_used_values.insert( pair< string, string >( t_it->first, t_it->second ) );
            f_unused_values.erase( t_it );
            return t_value;
        }

        throw error() << "requested argument <" << p_key << "> not found";
        stringstream t_converter;
        t_converter << t_it->second;
        t_converter >> t_value;
        return t_value;
    }

}

#endif
