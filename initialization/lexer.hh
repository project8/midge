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

            void execute( const string& p_file );

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
            //state
            //*****

        private:
            void parse_value();

            void parse_key();
            void parse_string();

            void parse_numerical_mantissa_sign();
            void parse_numerical_mantissa_pre();
            void parse_numerical_decimal();
            void parse_numerical_mantissa_post();
            void parse_numerical_power();
            void parse_numerical_exponent_sign();
            void parse_numerical_exponent_value();

            void parse_object_head();
            void parse_object_next();
            void parse_object_mid();
            void parse_object_stop();

            void parse_array_head();
            void parse_array_next();
            void parse_array_stop();

            void parse_error( error p_error );

            stack< void (lexer::*)() > f_states;
            token f_token;

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
