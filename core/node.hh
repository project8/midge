#ifndef _midge_node_hh_
#define _midge_node_hh_

#include "error.hh"

#include <map>
using std::map;

#include <string>
using std::string;

namespace midge
{

    class link;

    class node
    {
        public:
            node();
            virtual ~node();

        public:
            void set_name( const string& p_name );
            const string& get_name() const;

        private:
            string f_name;

        public:
            virtual void initialize() = 0;
            virtual void execute() = 0;
            virtual void finalize() = 0;

        public:
            link* input( const string& p_label );
            link* output( const string& p_label );

        protected:
            template< class x_parent, class x_child >
            void input( x_parent* p_parent, void (x_parent::*p_member)( x_child* ), const string& p_label );

            template< class x_parent, class x_child >
            void output( x_parent* p_parent, void (x_parent::*p_member)( x_child* ), const string& p_label );

        private:
            typedef map< string, link* > link_map;
            typedef link_map::iterator link_it;
            typedef link_map::const_iterator link_cit;
            typedef link_map::value_type link_entry;

            link_map f_input_map;
            link_map f_output_map;
    };

}

#include "member.hh"

namespace midge
{

    template< class x_parent, class x_child >
    void node::input( x_parent* p_parent, void (x_parent::*p_member)( x_child* ), const string& p_label )
    {
        link_it t_it = f_input_map.find( p_label );
        if( t_it == f_input_map.end() )
        {
            link* t_input = new member< x_parent*, void (x_parent::*)( x_child* ), x_child* >( p_parent, p_member );
            t_input->set_name( p_label );
            f_input_map.insert( link_entry( p_label, t_input ) );
        }
        else
        {
            throw error() << "node already has input named <" << p_label << ">";
        }
        return;
    }

    template< class x_parent, class x_child >
    void node::output( x_parent* p_parent, void (x_parent::*p_member)( x_child* ), const string& p_label )
    {
        link_it t_it = f_output_map.find( p_label );
        if( t_it == f_output_map.end() )
        {
            link* t_output = new member< x_parent*, void (x_parent::*)( x_child* ), x_child* >( p_parent, p_member );
            t_output->set_name( p_label );
            f_output_map.insert( link_entry( p_label, t_output ) );
        }
        else
        {
            throw error() << "node already has output named <" << p_label << ">";
        }
        return;
    }

}

#endif
