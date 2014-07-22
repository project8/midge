#include "reporter.hh"

#include "object.hh"
#include "array.hh"
#include "lingual.hh"
#include "numerical.hh"
#include "boolean.hh"
#include "null.hh"

#include <iostream>
using std::cout;
using std::endl;

namespace midge
{

    reporter::reporter() :
            f_contexts()
    {
    }
    reporter::~reporter()
    {
    }

    void reporter::operator()( value* p_value )
    {
        process_start();
        dispatch( p_value );
        process_stop();
        return;
    }
    void reporter::dispatch( value* p_value )
    {
        if( p_value->is< object >() == true )
        {
            object* t_object = p_value->as< object >();

            process_object_start();

            pair< string, value* > t_value;
            for( count_t t_index = 0; t_index < t_object->size(); t_index++ )
            {
                t_value = t_object->at( t_index );
                process_key( t_value.first );
                dispatch( t_value.second );
            }

            process_object_stop();

            return;
        }

        if( p_value->is< array >() == true )
        {
            array* t_array = p_value->as< array >();

            process_array_start();

            value* t_value;
            for( count_t t_index = 0; t_index < t_array->size(); t_index++ )
            {
                t_value = t_array->at( t_index );
                dispatch( t_value );
            }

            process_array_stop();

            return;
        }

        if( p_value->is< lingual >() == true )
        {
            process_lingual( p_value->as< lingual >()->str() );
            return;
        }

        if( p_value->is< numerical >() == true )
        {
            process_numerical( p_value->as< numerical >()->str() );
            return;
        }

        if( p_value->is< boolean >() == true )
        {
            process_boolean( p_value->as< boolean >()->str() );
            return;
        }

        if( p_value->is< null >() == true )
        {
            process_null();
            return;
        }

        throw error() << "reporter tried to dispatch unknown object type";
        return;
    }

    void reporter::process_key( string p_string )
    {
        f_contexts.top().key.assign( string( "\"" ) + p_string + string( "\" : " ) );
        return;
    }
    void reporter::process_lingual( string p_string )
    {
        cout << f_contexts.top().comma << "\n" << f_contexts.top().pad << f_contexts.top().key << "\"" << p_string << "\"";
        f_contexts.top().key.assign( "" );
        f_contexts.top().comma.assign( "," );
        return;
    }
    void reporter::process_numerical( string p_string )
    {
        cout << f_contexts.top().comma << "\n" << f_contexts.top().pad << f_contexts.top().key << p_string;
        f_contexts.top().key.assign( "" );
        f_contexts.top().comma.assign( "," );
        return;
    }
    void reporter::process_boolean( string p_string )
    {
        cout << f_contexts.top().comma << "\n" << f_contexts.top().pad << f_contexts.top().key << (p_string[ 0 ] == '0' ? "false" : "true");
        f_contexts.top().key.assign( "" );
        f_contexts.top().comma.assign( "," );
        return;
    }
    void reporter::process_null()
    {
        cout << f_contexts.top().comma << "\n" << f_contexts.top().pad << f_contexts.top().key << "null";
        f_contexts.top().key.assign( "" );
        f_contexts.top().comma.assign( "," );
        return;
    }
    void reporter::process_object_start()
    {
        if( f_contexts.size() > 1 )
        {
            cout << f_contexts.top().comma << "\n";
        }
        if( f_contexts.top().key.size() != 0 )
        {
            cout << f_contexts.top().pad << f_contexts.top().key << "\n";
        }
        cout << f_contexts.top().pad << "{";
        f_contexts.top().key.assign( "" );
        f_contexts.top().comma.assign( "," );

        f_contexts.push( context() );
        f_contexts.top().pad.assign( 4 * (f_contexts.size() - 1), ' ' );
        return;
    }
    void reporter::process_object_stop()
    {
        f_contexts.pop();
        cout << "\n" << f_contexts.top().pad << f_contexts.top().key << "}";
        f_contexts.top().key.assign( "" );
        f_contexts.top().comma.assign( "," );
        return;
    }
    void reporter::process_array_start()
    {
        if( f_contexts.size() > 1 )
        {
            cout << f_contexts.top().comma << "\n";
        }
        if( f_contexts.top().key.size() != 0 )
        {
            cout << f_contexts.top().pad << f_contexts.top().key << "\n";
        }
        cout << f_contexts.top().pad << "[";
        f_contexts.top().key.assign( "" );
        f_contexts.top().comma.assign( "," );

        f_contexts.push( context() );
        f_contexts.top().pad.assign( 4 * (f_contexts.size() - 1), ' ' );
        return;
    }
    void reporter::process_array_stop()
    {
        f_contexts.pop();
        cout << "\n" << f_contexts.top().pad << f_contexts.top().key << "]";
        f_contexts.top().key.assign( "" );
        f_contexts.top().comma.assign( "," );
        return;
    }
    void reporter::process_start()
    {
        f_contexts.push( context() );
        return;
    }
    void reporter::process_stop()
    {
        f_contexts.pop();
        cout << endl;
        return;
    }

    reporter::context::context() :
            pad( "" ),
            key( "" ),
            comma( "" )
    {
    }
    reporter::context::~context()
    {
    }

}
