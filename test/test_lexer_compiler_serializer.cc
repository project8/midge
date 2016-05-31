#include "arguments.hh"
#include "compiler.hh"
#include "evaluator.hh"
#include "lexer.hh"
#include "serializer.hh"
#include <iostream>
#include "message_logger.hh"

using namespace midge;

using std::cout;
using std::endl;
using std::string;

int main( int p_count, char** p_values )
{
    arguments t_arguments;
    t_arguments.required( "input" );
    t_arguments.required( "output" );

    try
    {
        t_arguments.start( p_count, p_values );
    }
    catch( const error& t_error )
    {
        msg_error( msg, "an error occured in arguments:" << ret );
        msg_error( msg, "  " << t_error.what() << eom );
        return -1;
    }

    lexer t_lexer;
    evaluator t_evaluator( t_arguments );
    compiler t_compiler;

    t_evaluator.insert_after( &t_lexer );
    t_compiler.insert_after( &t_evaluator );

    try
    {
        t_lexer( t_arguments.value< string >( "input" ) );
    }
    catch( const error& t_error )
    {
        msg_error( msg, "an error occurred during lexing:" << ret );
        msg_error( msg, "  " << t_error.what() << eom );
        return -1;
    }

    try
    {
        t_arguments.stop();
    }
    catch( const error& t_error )
    {
        msg_error( msg, "an error occured in arguments:" << ret );
        msg_error( msg, "  " << t_error.what() << eom );
        return -1;
    }

    serializer( t_arguments.value< string >( "output" ) )( t_compiler() );

    return 0;
}
