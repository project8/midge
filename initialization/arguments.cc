#include "arguments.hh"

namespace midge
{
    arguments::arguments() :
            f_value( "" ),
            f_required_values(),
            f_optional_values(),
            f_used_values()
    {
    }
    arguments::~arguments()
    {
    }

    void arguments::required( const string& p_key )
    {
        set< string >::iterator trequiredIt = f_required_values.find( p_key );
        map< string, string >::iterator tOptionalIt = f_optional_values.find( p_key );
        if( tOptionalIt == f_optional_values.end() )
        {
            if( trequiredIt == f_required_values.end() )
            {
                f_required_values.insert( p_key );
                return;
            }
            else
            {
                throw error() << "key <" << p_key << "> already in required key list";
                return;
            }
        }
        else
        {
            throw error() << "key <" << p_key << "> already in optional key list";
            return;
        }
    }

    void arguments::optional( const string& p_key, const string& p_default )
    {
        set< string >::iterator trequiredIt = f_required_values.find( p_key );
        map< string, string >::iterator tOptionalIt = f_optional_values.find( p_key );
        if( trequiredIt == f_required_values.end() )
        {
            if( tOptionalIt == f_optional_values.end() )
            {
                f_optional_values.insert( pair< string, string >( p_key, p_default ) );
                return;
            }
            else
            {
                throw error() << "key <" << p_key << "> already in optional key list";
                return;
            }
        }
        else
        {
            throw error() << "key <" << p_key << "> already in required key list";
            return;
        }
    }

    void arguments::start( int p_count, char** p_values )
    {
        set< string > t_required_values = f_required_values;
        map< string, string > t_optional_values = f_optional_values;

        f_value = p_values[ 0 ];

        string t_argument;
        size_t t_pos;
        string t_key;
        string t_value;
        set< string >::iterator t_required_it;
        map< string, string >::iterator t_optional_it;
        for( int t_index = 1; t_index < p_count; t_index++ )
        {
            t_argument = string( p_values[ t_index ] );

            t_pos = t_argument.find_first_of( '=' );
            if( t_pos == string::npos )
            {
                throw error() << "argument <" << t_argument << "> is badly formed";
                return;
            }

            t_key = t_argument.substr( 0, t_pos );
            if( t_key.size() == 0 )
            {
                throw error() << "argument <" << t_argument << "> is badly formed";
                return;
            }

            t_value = t_argument.substr( t_pos + 1 );
            if( t_value.size() == 0 )
            {
                throw error() << "argument <" << t_argument << "> is badly formed";
                return;
            }

            t_required_it = t_required_values.find( t_key );
            if( t_required_it != t_required_values.end() )
            {
                f_used_values.insert( pair< string, string >( t_key, t_value ) );
                t_required_values.erase( t_required_it );
                continue;
            }

            t_optional_it = t_optional_values.find( t_key );
            if( t_optional_it != t_optional_values.end() )
            {
                f_used_values.insert( pair< string, string >( t_key, t_value ) );
                t_optional_values.erase( t_optional_it );
                continue;
            }

            f_unused_values.insert( pair< string, string >( t_key, t_value ) );
        }

        if( t_required_values.size() != 0 )
        {
            stringstream t_message;
            for( t_required_it = t_required_values.begin(); t_required_it != t_required_values.end(); t_required_it++ )
            {
                t_message << "required argument <" << (*t_required_it) << "> not found" << '\n';
            }
            throw error() << t_message.str();
            return;
        }

        for( t_optional_it = t_optional_values.begin(); t_optional_it != t_optional_values.end(); t_optional_it++ )
        {
            f_used_values.insert( pair< string, string >( t_optional_it->first, t_optional_it->second ) );
        }
        return;
    }

    void arguments::stop()
    {
        if( f_unused_values.size() != 0 )
        {
            stringstream t_message;
            for( map< string, string >::iterator t_it = f_unused_values.begin(); t_it != f_unused_values.end(); t_it++ )
            {
                t_message << "unrecognized argument <" << t_it->first << "> appeared" << '\n';
            }
            throw error() << t_message.str();
            return;
        }
    }

}
