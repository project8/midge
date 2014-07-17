#ifndef _midge_lexer_hh_
#define _midge_lexer_hh_

#include "processor.hh"

#include <stack>
using std::stack;

#include <string>
using std::string;

#include <fstream>
using std::ifstream;

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
            void parse_boolean();
            void parse_numerical();

            void parse_object_head();
            void parse_object_start();
            void parse_object_mid();
            void parse_object_stop();

            void parse_array_head();
            void parse_array_start();
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
            static const string s_true;
            static const string s_false;
            static const string s_object_start;
            static const string s_object_stop;
            static const string s_array_start;
            static const string s_array_stop;

            static const string s_whitespace_set;
            static const string s_uppercase_set;
            static const string s_lowercase_set;
            static const string s_numeral_set;
            static const string s_symbol_set;
            static const string s_string_set;
    };

}

#endif
