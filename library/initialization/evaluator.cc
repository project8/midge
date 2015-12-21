#include "evaluator.hh"

#include <stack>
using std::stack;

namespace midge
{

    const string evaluator::s_start_bracket = string( "<" );
    const string evaluator::s_end_bracket = string( ">" );

    evaluator::evaluator( const arguments& p_arguments ) :
                f_arguments( p_arguments )
    {
    }
    evaluator::~evaluator()
    {
    }

    void evaluator::process_lingual( string p_string )
    {
        if( f_child == NULL )
        {
            return;
        }
        else
        {
            f_child->process_lingual( evaluate( p_string ) );
            return;
        }
    }

    string evaluator::evaluate( const string& p_string )
    {
        string t_buffer;
        stack< string > t_buffer_stack;

        t_buffer_stack.push( "" );
        for( size_t t_index = 0; t_index < p_string.size(); t_index++ )
        {
            if( p_string[ t_index ] == s_start_bracket[ 0 ] )
            {
                t_buffer_stack.top() += t_buffer;
                t_buffer_stack.push( "" );
                t_buffer.clear();
                continue;
            }

            if( p_string[ t_index ] == s_end_bracket[ 0 ] )
            {
                t_buffer_stack.top() += t_buffer;
                t_buffer = t_buffer_stack.top();
                t_buffer_stack.pop();
                if( t_buffer_stack.size() == 0 )
                {
                    throw error() << "bracket matching problem at position <" << t_index << "> in string <" << p_string << ">";
                    return string( "" );
                }

                try
                {
                    t_buffer = f_arguments.value< string >( t_buffer );
                }
                catch( const error& p_error )
                {
                    throw error() << "expected argument <" << t_buffer << "> not found";
                    return string( "" );
                }

                t_buffer_stack.top() += t_buffer;
                t_buffer.clear();
                continue;
            }

            t_buffer.append( 1, p_string[ t_index ] );
        }
        t_buffer_stack.top() += t_buffer;

        if( t_buffer_stack.size() != 1 )
        {
            throw error() << "bracket matching problem at end of string <" << p_string << ">";
            return string( "" );
        }

        return t_buffer_stack.top();
    }

}
