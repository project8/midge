#include "lexer.hh"
#include "serializer.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

int main( int p_count, char** p_values )
{
    if( p_count < 3 )
    {
        cout << "usage:" << endl;
        cout << "  test_lexer_serializer <input filename> <output filename>" << endl;
        return (-1);
    }

    string t_input_file( p_values[ 1 ] );
    string t_output_file( p_values[ 2 ] );

    lexer t_lexer;
    serializer t_serializer( t_output_file );

    t_serializer.insert_after( &t_lexer );

    try
    {
        t_lexer( t_input_file );
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
