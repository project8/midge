#include "lexer.hh"

#define _midge_debug_lexer_ 0

#if _midge_debug_lexer_==1
#include <iostream>
using std::cout;
using std::endl;
#endif

namespace midge
{

    lexer::lexer() :
            f_stream(),
            f_file( "" ),
            f_line( 0 ),
            f_column( 0 ),
            f_char( '\0' ),
            f_states(),
            f_token()
    {
    }
    lexer::~lexer()
    {
    }

    //*******
    //control
    //*******

    void lexer::execute( const string& p_file )
    {
        f_stream.open( p_file, std::ios_base::in );
        f_file = p_file;
        f_line = 1;
        f_column = 1;
        f_char = f_stream.get();

        process_start();

        f_states.push( &lexer::parse_value );
        while( true )
        {
            ((this)->*(f_states.top()))();

            if( f_states.empty() )
            {
                break;
            }
        }

        process_stop();

        return;
    }

    void lexer::increment()
    {
        //if iterator is already the end, bail out
        if( at_end() )
        {
            return;
        }

        //calculate adjustments to the line and column numbers
        int t_column_change;
        int t_line_change;
        if( at_exactly( s_new_line ) == true )
        {
            //if the current character is a newline, a successful increment will make the column number 1 and the line number jump by one.
            t_column_change = 1 - f_column;
            t_line_change = 1;
        }
        else
        {
            //if the current character is not a newline, a successful increment will make the column number jump by one and the line number will stay the same.
            t_column_change = 1;
            t_line_change = 0;
        }

        //increment the iterator
        //initmsg_debug( "popping the iterator" << eom )
        f_char = f_stream.get();

        //make sure that incrementing didn't put the iterator at the end
        if( at_end() )
        {
            return;
        }

        //apply the calculated column and line adjustments
        f_column = f_column + t_column_change;
        f_line = f_line + t_line_change;

        //GET OUT
        return;
    }
    bool lexer::at_end()
    {
        //if iterator is at EOF, return true
        if( f_stream.good() == false )
        {
            return true;
        }
        return false;
    }
    bool lexer::at_one_of( const string& p_string )
    {
        //if iterator is at EOF, return false
        if( at_end() )
        {
            return false;
        }

        //if iterator is at a character contained in chars, return true
        if( p_string.find( f_char ) != string::npos )
        {
            return true;
        }

        //otherwise return false
        return false;
    }
    bool lexer::at_exactly( const string& p_string )
    {
        //if iterator is already at EOF, return false
        if( at_end() )
        {
            return false;
        }

        //match the string
        bool t_match = true;
        string::const_iterator t_it = p_string.begin();
        while( t_it != p_string.end() )
        {
            //grab a character
            //initmsg_debug( "file character is <" << f_char << ">, comparison character is <" << *t_it << ">" << eom )

            //if that puts us at the end, try to go back and return false
            if( at_end() )
            {
                //initmsg_debug( "hit the end" << eom )
                t_match = false;
                break;
            }
            //if a mismatch is found, try to go back and return false
            if( f_char != *t_it )
            {
                //initmsg_debug( "<" << f_char << "> seriously does not match <" << *t_it << ">" << eom )
                t_match = false;
                break;
            }

            //initmsg_debug( "popping the iterator" << eom )

            f_char = f_stream.get();
            t_it++;
        }

        //go back
        while( t_it != p_string.begin() )
        {
            //initmsg_debug( "unpopping the iterator" << eom )

            f_stream.unget();
            t_it--;
        }

        //reset to character that we were originally on
        f_stream.unget();
        f_char = f_stream.get();

        return t_match;
    }

    //*****
    //state
    //*****

    void lexer::parse_value()
    {
#if _midge_debug_lexer_==1
        cout << "in parse_value at <" << f_char << ">" << endl;
#endif

        //if at end, then check that stack is empty
        if( at_end() == true )
        {
            f_states.pop();
            if( f_states.empty() == false )
            {
                parse_error( error() << "lexer reached premature end" );
                return;
            }
        }

        //if at whitespace, then recurse
        if( at_one_of( s_whitespace_set ) == true )
        {
            increment();
            return;
        }

        //if at open brace, then object start
        if( at_exactly( s_object_start ) == true )
        {
            process_object_start();

            increment();
            f_states.pop();
            f_states.push( &lexer::parse_object_head );
            return;
        }

        //if at open bracket, then array start
        if( at_exactly( s_array_start ) == true )
        {
            process_array_start();

            increment();
            f_states.pop();
            f_states.push( &lexer::parse_array_head );
            return;
        }

        //if at quote, then parse string
        if( at_exactly( s_quote ) == true )
        {
            f_token.str().clear();

            increment();
            f_states.pop();
            f_states.push( &lexer::parse_string );
            return;
        }

        //if at true or false, then parse boolean
        if( (at_exactly( s_true ) == true) || (at_exactly( s_false ) == true) )
        {
            f_token.str().clear();

            //no increment for this type
            f_states.pop();
            f_states.push( &lexer::parse_boolean );
            return;
        }

        //if at minus sign or numeral, then parse numerical
        if( (at_exactly( s_minus ) == true) || (at_one_of( s_numeral_set ) == true) )
        {
            f_token.str().clear();

            f_states.pop();
            f_states.push( &lexer::parse_numerical );
            return;
        }

        //unknown
        parse_error( error() << "lexer encountered unexpected character <" << f_char << "> in parse_value" );
        return;
    }
    void lexer::parse_key()
    {
#if _midge_debug_lexer_==1
        cout << "in parse_key at <" << f_char << ">" << endl;
#endif

        //if at string, then update token and recurse
        if( at_one_of( s_string_set ) == true )
        {
            f_token.str().append( 1, f_char );

            increment();
            return;
        }

        //if at quote, then issue token and previous
        if( at_exactly( s_quote ) == true )
        {
            process_key( &f_token );

            increment();
            f_states.pop();
            return;
        }

        //unknown
        parse_error( error() << "lexer encountered unexpected character <" << f_char << "> in parse_key" );
        return;
    }
    void lexer::parse_string()
    {
#if _midge_debug_lexer_==1
        cout << "in parse_string at <" << f_char << ">" << endl;
#endif

        //if at string, then update token and recurse
        if( at_one_of( s_string_set ) == true )
        {
            f_token.str().append( 1, f_char );

            increment();
            return;
        }

        //if at quote, then issue token and previous
        if( at_exactly( s_quote ) == true )
        {
            process_string( &f_token );

            increment();
            f_states.pop();
            return;
        }

        //unknown
        parse_error( error() << "lexer encountered unexpected character <" << f_char << "> in parse_string" );
        return;
    }
    void lexer::parse_boolean()
    {
    }
    void lexer::parse_numerical()
    {
#if _midge_debug_lexer_==1
        cout << "in parse_numerical at <" << f_char << ">" << endl;
#endif

        //if at whitespace, then issue token and previous
        if( at_one_of( s_whitespace_set ) == true )
        {
            process_numerical( &f_token );

            f_states.pop();
            return;
        }

        //if at comma, then issue token and previous
        if( at_exactly( s_comma ) == true )
        {
            process_numerical( &f_token );

            f_states.pop();
            return;
        }

        //if at close brace, then issue token and previous
        if( at_exactly( s_object_stop ) == true )
        {
            process_numerical( &f_token );

            f_states.pop();
            return;
        }

        //if at close bracket, then issue token and previous
        if( at_exactly( s_array_stop ) == true )
        {
            process_numerical( &f_token );

            f_states.pop();
            return;
        }

        f_token.str().append( 1, f_char );
        increment();
        return;
    }
    void lexer::parse_object_head()
    {
#if _midge_debug_lexer_==1
        cout << "in parse_object_head at <" << f_char << ">" << endl;
#endif

        //if at whitespace, then recurse
        if( at_one_of( s_whitespace_set ) == true )
        {
            increment();
            return;
        }

        //if at quote, then parse key, then parse mid
        if( at_exactly( s_quote ) == true )
        {
            f_token.str().clear();

            increment();
            f_states.pop();
            f_states.push( &lexer::parse_object_mid );
            f_states.push( &lexer::parse_key );
            return;
        }

        //if at close brace, then previous
        if( at_exactly( s_object_stop ) == true )
        {
            process_object_stop();

            increment();
            f_states.pop();
            return;
        }

        //unknown
        parse_error( error() << "lexer encountered unexpected character <" << f_char << "> in parse_object_head" );
        return;
    }
    void lexer::parse_object_start()
    {
#if _midge_debug_lexer_==1
        cout << "in parse_object_start at <" << f_char << ">" << endl;
#endif

        //if at whitespace, then recurse
        if( at_one_of( s_whitespace_set ) == true )
        {
            increment();
            return;
        }

        //if at quote, then parse key, then parse object mid
        if( at_exactly( s_quote ) == true )
        {
            f_token.str().clear();

            increment();
            f_states.pop();
            f_states.push( &lexer::parse_object_mid );
            f_states.push( &lexer::parse_key );
            return;
        }

        //unknown
        parse_error( error() << "lexer encountered unexpected character <" << f_char << "> in parse_object_start" );
        return;
    }
    void lexer::parse_object_mid()
    {
#if _midge_debug_lexer_==1
        cout << "in parse_object_mid at <" << f_char << ">" << endl;
#endif

        //if at whitespace, then recurse
        if( at_one_of( s_whitespace_set ) == true )
        {
            increment();
            return;
        }

        //if at colon, then parse value, then parse object stop
        if( at_exactly( s_colon ) == true )
        {
            increment();
            f_states.pop();
            f_states.push( &lexer::parse_object_stop );
            f_states.push( &lexer::parse_value );
            return;
        }

        //unknown
        parse_error( error() << "lexer encountered unexpected character <" << f_char << "> in parse_object_mid" );
        return;
    }
    void lexer::parse_object_stop()
    {
#if _midge_debug_lexer_==1
        cout << "in parse_object_stop at <" << f_char << ">" << endl;
#endif

        //if at whitespace, then recurse
        if( at_one_of( s_whitespace_set ) == true )
        {
            increment();
            return;
        }

        //if at comma, then parse object start
        if( at_exactly( s_comma ) == true )
        {
            increment();
            f_states.pop();
            f_states.push( &lexer::parse_object_start );
            return;
        }

        //if at close brace, then previous
        if( at_exactly( s_object_stop ) == true )
        {
            process_object_stop();

            increment();
            f_states.pop();
            return;
        }

        //unknown
        parse_error( error() << "lexer encountered unexpected character <" << f_char << "> in parse_object_stop" );
        return;
    }
    void lexer::parse_array_head()
    {
    }
    void lexer::parse_array_start()
    {
    }
    void lexer::parse_array_stop()
    {
    }

    void lexer::parse_error( error p_error )
    {
        p_error << ":\n";
        p_error << "  file <" << f_file << ">\n";
        p_error << "  line <" << f_line << ">\n";
        p_error << "  column <" << f_column << ">";
        throw p_error;
        return;
    }

    //*******
    //lexicon
    //*******

    const string lexer::s_space = string( " " );
    const string lexer::s_tab = string( "\t" );
    const string lexer::s_new_line = string( "\n" );
    const string lexer::s_carriage_return = string( "\r" );

    const string lexer::s_quote = string( "\"" );
    const string lexer::s_colon = string( ":" );
    const string lexer::s_comma = string( "," );
    const string lexer::s_minus = string( "-" );
    const string lexer::s_true = string( "true" );
    const string lexer::s_false = string( "false" );
    const string lexer::s_object_start = string( "{" );
    const string lexer::s_object_stop = string( "}" );
    const string lexer::s_array_start = string( "[" );
    const string lexer::s_array_stop = string( "]" );

    const string lexer::s_whitespace_set = lexer::s_space + lexer::s_tab + lexer::s_new_line + lexer::s_carriage_return;
    const string lexer::s_uppercase_set = string( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
    const string lexer::s_lowercase_set = string( "abcdefghijklmnopqrstuvwxyz" );
    const string lexer::s_numeral_set = string( "0123456789" );
    const string lexer::s_symbol_set = string( "!@#$%^&*()-=_+[]\{}|;:,./<>?" );
    const string lexer::s_string_set = lexer::s_whitespace_set + lexer::s_uppercase_set + lexer::s_lowercase_set + lexer::s_numeral_set + lexer::s_symbol_set;

}
