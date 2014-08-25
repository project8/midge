#ifndef _midge_lexer_hh_
#define _midge_lexer_hh_

#include "processor.hh"

#include <stack>
using std::stack;

#include <string>
using std::string;

#include <fstream>
using std::ifstream;

//personal comment:
//
//  this is so shitty and arcane.
//  really this job should be done with NFA.

namespace midge
{

    class lexer :
        public processor
    {
        public:
            lexer();
            virtual ~lexer();

            void operator()( const string& p_file );

            //*******
            //control
            //*******

        private:
            void increment();
            bool at_end();
            bool at_one_of( const string& aSet );
            bool at_exactly( const string& aString );

            ifstream f_stream;
            string f_file;
            int f_line;
            int f_column;
            char f_char;

            //*****
            //command
            //*****

        private:
            void lex_value();

            void lex_key();
            void lex_lingual();

            void lex_numerical_mantissa_sign();
            void lex_numerical_mantissa_pre();
            void lex_numerical_decimal();
            void lex_numerical_mantissa_post();
            void lex_numerical_power();
            void lex_numerical_exponent_sign();
            void lex_numerical_exponent_value();

            void lex_object_head();
            void lex_object_next();
            void lex_object_mid();
            void lex_object_stop();

            void lex_array_head();
            void lex_array_next();
            void lex_array_stop();

            void lex_error( error p_error );

            stack< void (lexer::*)() > f_states;
            string f_buffer;

            //*******
            //lexicon
            //*******

        private:
            static const string s_space;
            static const string s_tab;
            static const string s_new_line;
            static const string s_carriage_return;

            static const string s_quote;
            static const string s_colon;
            static const string s_comma;
            static const string s_minus;
            static const string s_decimal;
            static const string s_true;
            static const string s_false;
            static const string s_null;
            static const string s_object_start;
            static const string s_object_stop;
            static const string s_array_start;
            static const string s_array_stop;

            static const string s_power_set;
            static const string s_whitespace_set;
            static const string s_uppercase_set;
            static const string s_lowercase_set;
            static const string s_numeral_set;
            static const string s_symbol_set;
            static const string s_string_set;
    };

}

#endif
