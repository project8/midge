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
            f_buffer()
    {
    }
    lexer::~lexer()
    {
    }

    //*******
    //control
    //*******

    void lexer::operator()( const string& p_file )
    {
        f_stream.open( p_file, std::ios_base::in );
        f_file = p_file;
        f_line = 1;
        f_column = 1;
        f_char = f_stream.get();

        process_start();

        f_states.push( &lexer::lex_value );
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

    void lexer::lex_value()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_value at <" << f_char << ">" << endl;
#endif
        //if at whitespace, then increment, then recurse
        if( at_one_of( s_whitespace_set ) == true )
        {
            increment();

            return;
        }

        //if at open brace, then process object start, then increment, then parse object head
        else if( at_exactly( s_object_start ) == true )
        {
            process_object_start();

            increment();

            f_states.pop();
            f_states.push( &lexer::lex_object_head );
            return;
        }

        //if at open bracket, then process array start, then increment, then parse array head
        else if( at_exactly( s_array_start ) == true )
        {
            process_array_start();

            increment();
            f_states.pop();
            f_states.push( &lexer::lex_array_head );
            return;
        }

        //if at quote, then clear token, then increment, then parse string
        else if( at_exactly( s_quote ) == true )
        {
            f_buffer.clear();

            increment();

            f_states.pop();
            f_states.push( &lexer::lex_lingual );
            return;
        }

        //if at minus, then clear token, then parse numerical mantissa sign
        else if( at_exactly( s_minus ) == true )
        {
            f_buffer.clear();

            f_states.pop();
            f_states.push( &lexer::lex_numerical_mantissa_sign );
            return;
        }

        //if at numeral, then clear token, then parse numerical mantissa pre
        else if( at_one_of( s_numeral_set ) == true )
        {
            f_buffer.clear();

            f_states.pop();
            f_states.push( &lexer::lex_numerical_mantissa_pre );
            return;
        }

        //if at true, then assign token, then process boolean, then increment, then previous
        else if( at_exactly( s_true ) == true )
        {
            f_buffer.assign( 1, '1' );

            process_boolean( f_buffer );

            for( size_t t_index = 0; t_index < s_true.size(); t_index++ )
            {
                increment();
            }
            f_states.pop();
            return;
        }

        //if at false, then assign token, then process boolean, then increment, then previous
        else if( at_exactly( s_false ) == true )
        {
            f_buffer.assign( 1, '0' );

            process_boolean( f_buffer );

            for( size_t t_index = 0; t_index < s_false.size(); t_index++ )
            {
                increment();
            }
            f_states.pop();
            return;
        }

        //if at null, then process null, then increment, then previous
        else if( at_exactly( s_null ) == true )
        {
            process_null();

            for( size_t t_index = 0; t_index < s_null.size(); t_index++ )
            {
                increment();
            }
            f_states.pop();
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_value" );
            return;
        }
    }

    void lexer::lex_key()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_key at <" << f_char << ">" << endl;
#endif

        //if at string, then append token, then increment, then recurse
        if( at_one_of( s_string_set ) == true )
        {
            f_buffer.append( 1, f_char );

            increment();

            return;
        }

        //if at quote, then process key, then increment, then previous
        else if( at_exactly( s_quote ) == true )
        {
            process_key( f_buffer );

            increment();

            f_states.pop();
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_key" );
            return;
        }
    }
    void lexer::lex_lingual()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_string at <" << f_char << ">" << endl;
#endif

        //if at string, then append token, then increment, then recurse
        if( at_one_of( s_string_set ) == true )
        {
            f_buffer.append( 1, f_char );

            increment();

            return;
        }

        //if at quote, then process string, then increment, then previous
        else if( at_exactly( s_quote ) == true )
        {
            process_lingual( f_buffer );

            increment();

            f_states.pop();
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_string" );

            return;
        }
    }

    void lexer::lex_numerical_mantissa_sign()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_numerical_mantissa_sign at <" << f_char << ">" << endl;
#endif

        //update token and increment
        f_buffer.append( 1, f_char );
        increment();

        //if at numeral, then parse numerical mantissa pre
        if( at_one_of( s_numeral_set ) == true )
        {
            f_states.pop();
            f_states.push( &lexer::lex_numerical_mantissa_pre );
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_numerical_mantissa_sign" );
            return;
        }
    }
    void lexer::lex_numerical_mantissa_pre()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_numerical_mantissa_pre at <" << f_char << ">" << endl;
#endif

        //update token and increment
        f_buffer.append( 1, f_char );
        increment();

        //if at numeral, then recurse
        if( at_one_of( s_numeral_set ) == true )
        {
            return;
        }

        //if at decimal, then parse numerical mantissa decimal
        else if( at_exactly( s_decimal ) == true )
        {
            f_states.pop();
            f_states.push( &lexer::lex_numerical_decimal );
            return;
        }

        //if at power, then parse numerical power
        else if( at_one_of( s_power_set ) == true )
        {
            f_states.pop();
            f_states.push( &lexer::lex_numerical_power );
            return;
        }

        //if at whitespace, comma, close brace or close bracket, then process numerical, then previous
        else if( (at_one_of( s_whitespace_set ) == true) || (at_exactly( s_comma ) == true) || (at_exactly( s_object_stop ) == true) || (at_exactly( s_array_stop ) == true) )
        {
            process_numerical( f_buffer );

            f_states.pop();
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_numerical_mantissa_pre" );
            return;
        }
    }
    void lexer::lex_numerical_decimal()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_numerical_decimal at <" << f_char << ">" << endl;
#endif

        //update token and increment
        f_buffer.append( 1, f_char );
        increment();

        //if at numeral, then parse numerical mantissa post
        if( at_one_of( s_numeral_set ) == true )
        {
            f_states.pop();
            f_states.push( &lexer::lex_numerical_mantissa_post );
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_numerical_decimal" );
            return;
        }
    }
    void lexer::lex_numerical_mantissa_post()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_numerical_mantissa_post at <" << f_char << ">" << endl;
#endif

        //update token and increment
        f_buffer.append( 1, f_char );
        increment();

        //if at numeral, then recurse
        if( at_one_of( s_numeral_set ) == true )
        {
            return;
        }

        //if at power, then parse numerical power
        else if( at_one_of( s_power_set ) == true )
        {
            f_states.pop();
            f_states.push( &lexer::lex_numerical_power );
            return;
        }

        //if at whitespace, comma, close brace or close bracket, then process numerical, then previous
        else if( (at_one_of( s_whitespace_set ) == true) || (at_exactly( s_comma ) == true) || (at_exactly( s_object_stop ) == true) || (at_exactly( s_array_stop ) == true) )
        {
            process_numerical( f_buffer );

            f_states.pop();
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_numerical_mantissa_post" );
            return;
        }
    }
    void lexer::lex_numerical_power()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_numerical_power at <" << f_char << ">" << endl;
#endif

        //update token and increment
        f_buffer.append( 1, f_char );
        increment();

        //if at minus, then parse numerical exponent value
        if( at_exactly( s_minus ) == true )
        {
            f_states.pop();
            f_states.push( &lexer::lex_numerical_exponent_sign );
            return;
        }

        //if at numeral, then parse numerical exponent value
        if( at_one_of( s_numeral_set ) == true )
        {
            f_states.pop();
            f_states.push( &lexer::lex_numerical_exponent_value );
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_numerical_power" );
            return;
        }
    }
    void lexer::lex_numerical_exponent_sign()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_numerical_exponent_sign at <" << f_char << ">" << endl;
#endif

        //update token and increment
        f_buffer.append( 1, f_char );
        increment();

        //if at numeral, then parse numerical exponent value
        if( at_one_of( s_numeral_set ) == true )
        {
            f_states.pop();
            f_states.push( &lexer::lex_numerical_exponent_value );
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_numerical_exponent_sign" );
            return;
        }
    }
    void lexer::lex_numerical_exponent_value()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_numerical_exponent_value at <" << f_char << ">" << endl;
#endif

        //update token and increment
        f_buffer.append( 1, f_char );
        increment();

        //if at numeral, then recurse
        if( at_one_of( s_numeral_set ) == true )
        {
            return;
        }

        //if at whitespace, comma, close brace or close bracket, then process numerical, then previous
        else if( (at_one_of( s_whitespace_set ) == true) || (at_exactly( s_comma ) == true) || (at_exactly( s_object_stop ) == true) || (at_exactly( s_array_stop ) == true) )
        {
            process_numerical( f_buffer );

            f_states.pop();
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_numerical_exponent_value" );
            return;
        }
    }

    void lexer::lex_object_head()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_object_head at <" << f_char << ">" << endl;
#endif

        //if at whitespace, then increment, then recurse
        if( at_one_of( s_whitespace_set ) == true )
        {
            increment();

            return;
        }

        //if at quote, then clear token, then increment, then parse key, then parse object mid
        else if( at_exactly( s_quote ) == true )
        {
            f_buffer.clear();

            increment();

            f_states.pop();
            f_states.push( &lexer::lex_object_mid );
            f_states.push( &lexer::lex_key );
            return;
        }

        //if at close brace, then previous
        else if( at_exactly( s_object_stop ) == true )
        {
            process_object_stop();

            increment();
            f_states.pop();
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_object_head" );
            return;
        }
    }
    void lexer::lex_object_next()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_object_next at <" << f_char << ">" << endl;
#endif

        //if at whitespace, then recurse
        if( at_one_of( s_whitespace_set ) == true )
        {
            increment();
            return;
        }

        //if at quote, then clear token, then increment, then parse key, then parse object mid
        else if( at_exactly( s_quote ) == true )
        {
            f_buffer.clear();

            increment();
            f_states.pop();
            f_states.push( &lexer::lex_object_mid );
            f_states.push( &lexer::lex_key );
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_object_next" );
            return;
        }
    }
    void lexer::lex_object_mid()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_object_mid at <" << f_char << ">" << endl;
#endif

        //if at whitespace, then recurse
        if( at_one_of( s_whitespace_set ) == true )
        {
            increment();
            return;
        }

        //if at colon, then increment, then parse value, then parse object stop
        else if( at_exactly( s_colon ) == true )
        {
            increment();

            f_states.pop();
            f_states.push( &lexer::lex_object_stop );
            f_states.push( &lexer::lex_value );
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_object_mid" );
            return;
        }
    }
    void lexer::lex_object_stop()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_object_stop at <" << f_char << ">" << endl;
#endif

        //if at whitespace, then increment, then recurse
        if( at_one_of( s_whitespace_set ) == true )
        {
            increment();

            return;
        }

        //if at comma, then increment, then parse object next
        else if( at_exactly( s_comma ) == true )
        {
            increment();

            f_states.pop();
            f_states.push( &lexer::lex_object_next );
            return;
        }

        //if at close brace, then previous
        else if( at_exactly( s_object_stop ) == true )
        {
            process_object_stop();

            increment();
            f_states.pop();
            return;
        }

        //if at anything else, then error
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_object_stop" );
            return;
        }
    }

    void lexer::lex_array_head()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_array_head at <" << f_char << ">" << endl;
#endif

        //if at whitespace, then increment, then recurse
        if( at_one_of( s_whitespace_set ) == true )
        {
            increment();
            return;
        }

        //if at close brace, then process array stop, then increment, then previous
        else if( at_exactly( s_array_stop ) == true )
        {
            process_array_stop();

            increment();

            f_states.pop();
            return;
        }

        //if at anything else, then parse value, then parse array stop
        else
        {
            f_states.pop();
            f_states.push( &lexer::lex_array_stop );
            f_states.push( &lexer::lex_value );
            return;
        }
    }
    void lexer::lex_array_next()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_array_next at <" << f_char << ">" << endl;
#endif

        //if at whitespace, then increment, then recurse
        if( at_one_of( s_whitespace_set ) == true )
        {
            increment();
            return;
        }

        //if at anything else, then parse value, then parse array stop
        else
        {
            f_states.pop();
            f_states.push( &lexer::lex_array_stop );
            f_states.push( &lexer::lex_value );
            return;
        }
    }
    void lexer::lex_array_stop()
    {
#if _midge_debug_lexer_==1
        cout << "in lex_array_stop at <" << f_char << ">" << endl;
#endif

        //if at whitespace, then increment, then recurse
        if( at_one_of( s_whitespace_set ) == true )
        {
            increment();

            return;
        }

        //if at comma, then increment, then parse array next
        else if( at_exactly( s_comma ) == true )
        {
            increment();

            f_states.pop();
            f_states.push( &lexer::lex_array_next );
            return;
        }

        //if at close bracket, then process array stop, then increment, then previous
        else if( at_exactly( s_array_stop ) == true )
        {
            process_array_stop();

            increment();

            f_states.pop();
            return;
        }

        //if at anything else, then parse value, then parse array stop
        else
        {
            lex_error( error() << "lexer encountered unexpected character <" << f_char << "> in lex_array_stop" );
            return;
        }
    }

    void lexer::lex_error( error p_error )
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
    const string lexer::s_decimal = string( "." );
    const string lexer::s_true = string( "true" );
    const string lexer::s_false = string( "false" );
    const string lexer::s_null = string( "null" );
    const string lexer::s_object_start = string( "{" );
    const string lexer::s_object_stop = string( "}" );
    const string lexer::s_array_start = string( "[" );
    const string lexer::s_array_stop = string( "]" );

    const string lexer::s_power_set = string( "eE" );
    const string lexer::s_whitespace_set = lexer::s_space + lexer::s_tab + lexer::s_new_line + lexer::s_carriage_return;
    const string lexer::s_uppercase_set = string( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
    const string lexer::s_lowercase_set = string( "abcdefghijklmnopqrstuvwxyz" );
    const string lexer::s_numeral_set = string( "0123456789" );
    const string lexer::s_symbol_set = string( "!@#$%^&*()-=_+[]\{}|;:,./<>?" );
    const string lexer::s_string_set = lexer::s_whitespace_set + lexer::s_uppercase_set + lexer::s_lowercase_set + lexer::s_numeral_set + lexer::s_symbol_set;

}
