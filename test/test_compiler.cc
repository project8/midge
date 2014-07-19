#include "lexer.hh"
#include "compiler.hh"
#include "reporter.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

int main( int p_count, char** p_values )
{
    if( p_count < 2 )
    {
        cout << "usage:" << endl;
        cout << "  test_compiler <input filename>" << endl;
        return (-1);
    }

    string t_input_file( p_values[ 1 ] );

    lexer t_lexer;
    compiler t_compiler;
    reporter t_reporter;

    t_compiler.insert_after( &t_lexer );
    t_reporter.insert_after( &t_compiler );

    try
    {
        t_lexer.input( t_input_file );
    }
    catch( const error& t_error )
    {
        cout << "an error occurred:" << endl;
        cout << "  " << t_error.what() << endl;
        return -1;
    }
    catch( ... )
    {
        cout << "an unknown error occurred" << endl;
        return -1;
    }

    return 0;
}
