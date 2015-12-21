#include "arguments.hh"
#include "evaluator.hh"
#include "lexer.hh"
#include "reporter.hh"
#include "message.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

int main( int p_count, char** p_values )
{
    arguments t_arguments;
    t_arguments.required( "input" );

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
    reporter t_reporter;

    t_evaluator.insert_after( &t_lexer );
    t_reporter.insert_after( &t_evaluator );

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

    return 0;
}
