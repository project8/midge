#include "lexer.hh"
#include "processor_terminal.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

int main( int p_count, char** p_values )
{
    if( p_count < 2 )
    {
        cout << "usage:" << endl;
        cout << "  test_lexer <filename>" << endl;
        return (-1);
    }

    string t_file( p_values[ 1 ] );

    lexer t_lexer;
    processor_terminal t_processor_terminal;

    t_processor_terminal.insert_after( &t_lexer );

    try
    {
        t_lexer.execute( t_file );
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
