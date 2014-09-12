#include "lexer.hh"
#include "compiler.hh"
#include "coremsg.hh"
#include "midge_builder.hh"
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

    msg_normal( coremsg, "welcome to midge" << eom );
    msg_normal( coremsg, "processing..." << eom );

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
        msg_error( coremsg, "error occurred during input:" << ret );
        msg_error( coremsg, "  " << t_error.what() << eom );
        return -1;
    }
    catch( const exception& t_exception )
    {
        msg_error( coremsg, "exception occurred during execution:" << ret );
        msg_error( coremsg, "  " << t_exception.what() << eom );
        return -1;
    }
    catch( ... )
    {
        msg_error( coremsg, "unknown error occurred during input" << eom );
        return -1;
    }

    midge_builder t_builder;

    try
    {
        t_builder( t_compiler() );
    }
    catch( const error& t_error )
    {
        msg_error( coremsg, "error occurred during execution:" << ret );
        msg_error( coremsg, "  " << t_error.what() << eom );
        return -1;
    }
    catch( const exception& t_exception )
    {
        msg_error( coremsg, "exception occurred during execution:" << ret );
        msg_error( coremsg, "  " << t_exception.what() << eom );
        return -1;
    }
    catch( ... )
    {
        msg_error( coremsg, "unknown error occurred during execution" << eom );
        return -1;
    }

    msg_normal( coremsg, "...processing complete" << eom );
    msg_normal( coremsg, "goodbye" << eom );

    return 0;
}
