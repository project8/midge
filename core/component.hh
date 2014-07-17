#ifndef _midge_component_hh_
#define _midge_component_hh_

#include "node.hh"
#include "member.hh"

#include <map>
using std::map;

namespace midge
{

    class component :
        public node
    {
        public:
            component();
            virtual ~component();

        public:
            link* input( const string& p_label );
            link* output( const string& p_label );

        private:
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

    template< class x_parent, class x_child >
    void component::input( x_parent* p_parent, void (x_parent::*p_member)( x_child* ), const string& p_label )
    {
        link_it t_it = f_input_map.find( p_label );
        if( t_it != f_input_map.end() )
        {
            throw error() << "component named <" << get_name() << "> already has input named <" << p_label << ">";
        }

        f_input_map.insert( link_entry( p_label, new member< x_parent*, void (x_parent::*)( x_child* ), x_child* >( p_parent, p_member ) ) );

        return;
    }

    template< class x_parent, class x_child >
    void component::output( x_parent* p_parent, void (x_parent::*p_member)( x_child* ), const string& p_label )
    {
        link_it t_it = f_output_map.find( p_label );
        if( t_it != f_output_map.end() )
        {
            throw error() << "component named <" << get_name() << "> already has output named <" << p_label << ">";
        }

        f_output_map.insert( link_entry( p_label, new member< x_parent*, void (x_parent::*)( x_child* ), x_child* >( p_parent, p_member ) ) );

        return;
    }

}

#endif
