#include "node.hh"

namespace midge
{

    node::node() :
            f_name( "" ),
            f_input_map(),
            f_output_map()
    {
    }
    node::~node()
    {
        link_it t_it;

        for( t_it = f_input_map.begin(); t_it != f_input_map.end(); t_it++ )
        {
            delete (t_it->second);
        }

        for( t_it = f_output_map.begin(); t_it != f_output_map.end(); t_it++ )
        {
            delete (t_it->second);
        }
    }

    const string& node::get_name() const
    {
        return f_name;
    }
    void node::set_name( const string& p_name )
    {
        f_name = p_name;
        return;
    }

    link* node::input( const string& p_label )
    {
        link_it t_it = f_input_map.find( p_label );
        if( f_input_map.find( p_label ) == f_input_map.end() )
        {
            throw error() << "node named <" << get_name() << "> has no input named <" << p_label << ">";
            return NULL;
        }
        return t_it->second;
    }
    link* node::output( const string& p_label )
    {
        link_it t_it = f_output_map.find( p_label );
        if( f_output_map.find( p_label ) == f_output_map.end() )
        {
            throw error() << "node named <" << get_name() << "> has no output named <" << p_label << ">";
            return NULL;
        }
        return t_it->second;
    }

}
