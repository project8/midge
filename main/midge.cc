#include "lexer.hh"
#include "compiler.hh"
#include "root_builder.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

int main( int p_count, char** p_values )
{
    if( p_count < 2 )
    {
        cout << "usage:" << endl;
        cout << "  midge <input filename>" << endl;
        return (-1);
    }

    string t_input_file( p_values[ 1 ] );

    lexer t_lexer;
    compiler t_compiler;

    t_compiler.insert_after( &t_lexer );

    try
    {
        t_lexer( t_input_file );
    }
    catch( const error& t_error )
    {
        cout << "error occurred during input:" << endl;
        cout << "  " << t_error.what() << endl;
        return -1;
    }
    catch( ... )
    {
        cout << "an unknown error occurred during input" << endl;
        return -1;
    }

    root_builder t_builder;

    try
    {
        t_builder( t_compiler() );
    }
    catch( const error& t_error )
    {
        cout << "error occurred during execution:" << endl;
        cout << "  " << t_error.what() << endl;
        return -1;
    }
    catch( ... )
    {
        cout << "an unknown error occurred during execution" << endl;
        return -1;
    }

    return 0;
}
