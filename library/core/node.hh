#ifndef _midge_node_hh_
#define _midge_node_hh_

#include <map>

#include "cancelable.hh"
#include "input.hh"
#include "output.hh"

#include "factory.hh"

#include <string>

namespace midge
{

    class node : public cancelable
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
            input* in( const std::string& p_label );
            output* out( const std::string& p_label );

        protected:
            void in( input*, const std::string& p_label );
            void out( output*, const std::string& p_label );

        private:
            typedef std::map< std::string, input* > input_map;
            typedef input_map::iterator input_it;
            typedef input_map::const_iterator input_cit;
            typedef input_map::value_type input_entry;

            input_map f_input_map;

            typedef std::map< std::string, output* > output_map;
            typedef output_map::iterator output_it;
            typedef output_map::const_iterator output_cit;
            typedef output_map::value_type output_entry;

            output_map f_output_map;
    };

}

#define REGISTER_NODE( node_class, node_name ) \
        static ::scarab::registrar< ::midge::node, node_class > s_node_##node_class##_registrar( node_name );

#endif
