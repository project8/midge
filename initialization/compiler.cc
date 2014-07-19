#include "compiler.hh"

#include "object.hh"
#include "array.hh"
#include "lingual.hh"
#include "numerical.hh"
#include "boolean.hh"
#include "null.hh"

namespace midge
{

    compiler::compiler() :
            f_result( NULL ),
            f_values()
    {
    }
    compiler::~compiler()
    {
        delete f_result;
    }

    value* compiler::operator()()
    {
        return f_result;
    }

    void compiler::process_key( string p_string )
    {
        f_key = p_string;
        return;
    }
    void compiler::process_lingual( string p_string )
    {
        add_single( new lingual( p_string ) );
        return;
    }
    void compiler::process_numerical( string p_string )
    {
        add_single( new numerical( p_string ) );
        return;
    }
    void compiler::process_boolean( string p_string )
    {
        add_single( new boolean( p_string ) );
        return;
    }
    void compiler::process_null()
    {
        add_single( new null() );
        return;
    }
    void compiler::process_object_start()
    {
        add_composite( new object() );
        return;
    }
    void compiler::process_object_stop()
    {
        f_result = f_values.top();
        f_values.pop();
        return;
    }
    void compiler::process_array_start()
    {
        add_composite( new array() );
        return;
    }
    void compiler::process_array_stop()
    {
        f_result = f_values.top();
        f_values.pop();
        return;
    }
    void compiler::process_start()
    {
        delete f_result;
        return;
    }
    void compiler::process_stop()
    {
        while( f_values.size() != 0 )
        {
            delete f_values.top();
            f_values.pop();
        }
        return;
    }

    void compiler::add_single( value* p_value )
    {
        if( f_values.empty() == true )
        {
            f_values.push( p_value );
            return;
        }

        if( f_values.top()->is< object >() )
        {
            f_values.top()->as< object >()->add( f_key, p_value );
            return;
        }

        if( f_values.top()->is< array >() )
        {
            f_values.top()->as< array >()->add( p_value );
            return;
        }

        throw error() << "compiler tried to process unknown value type";
        return;
    }
    void compiler::add_composite( value* p_value )
    {
        if( f_values.empty() == true )
        {
            f_values.push( p_value );
            return;
        }

        if( f_values.top()->is< object >() )
        {
            f_values.top()->as< object >()->add( f_key, p_value );
            f_values.push( p_value );
            return;
        }

        if( f_values.top()->is< array >() )
        {
            f_values.top()->as< array >()->add( p_value );
            f_values.push( p_value );
            return;
        }

        throw error() << "compiler tried to process unknown value type";
        return;
    }

}
