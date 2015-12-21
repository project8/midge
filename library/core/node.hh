#ifndef _midge_node_hh_
#define _midge_node_hh_

#include <map>

#include "input.hh"
#include "output.hh"
using std::map;

#include <string>
using std::string;

namespace midge
{

    class node
    {
        public:
            node();
            virtual ~node();

        public:
            accessible( string_t, name );

        public:
            virtual void initialize() = 0;
            virtual void execute() = 0;
            virtual void finalize() = 0;

        public:
            input* in( const string& p_label );
            output* out( const string& p_label );

        protected:
            void in( input*, const string& p_label );
            void out( output*, const string& p_label );

        private:
            typedef map< string, input* > input_map;
            typedef input_map::iterator input_it;
            typedef input_map::const_iterator input_cit;
            typedef input_map::value_type input_entry;

            input_map f_input_map;

            typedef map< string, output* > output_map;
            typedef output_map::iterator output_it;
            typedef output_map::const_iterator output_cit;
            typedef output_map::value_type output_entry;

            output_map f_output_map;
    };

}

#endif
