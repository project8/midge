#ifndef _midge_node_hh_
#define _midge_node_hh_

#include "diptera.hh"
#include "input.hh"
#include "instructable.hh"
#include "output.hh"
#include "shared_cancel.hh"

#include "cancelable.hh"
#include "factory.hh"

#include <map>
#include <string>

namespace midge
{

    class node : public scarab::cancelable,  public instructable
    {
        public:
            node();
            virtual ~node();

        public:
            mv_accessible( string_t, name );

        public:
            virtual void initialize() = 0;
            virtual void execute( diptera* ) = 0;
            virtual void finalize() = 0;

        public:
            node* node_ptr( const std::string& p_label );
            input* in( const std::string& p_label );
            output* out( const std::string& p_label );

            void node_ptr( node*, const std::string& p_label );

        protected:
            void in( input*, const std::string& p_label );
            void out( output*, const std::string& p_label );

        private:
            typedef std::map< std::string, node* > node_map;
            typedef node_map::iterator node_it;
            typedef node_map::const_iterator node_cit;
            typedef node_map::value_type node_entry;

            node_map f_node_map;

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

        protected:
            void do_cancellation() override;
    };

}

#define REGISTER_NODE( node_class, node_name ) \
        static ::scarab::registrar< ::midge::node, node_class > s_node_##node_class##_registrar( node_name );

#endif
