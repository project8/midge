#include "node.hh"

namespace midge
{

    node::node() :
            f_name( "" ),
            f_in_map(),
            f_out_map()
    {
    }
    node::~node()
    {
        link_it t_it;

        for( t_it = f_in_map.begin(); t_it != f_in_map.end(); t_it++ )
        {
            delete (t_it->second);
        }

        for( t_it = f_out_map.begin(); t_it != f_out_map.end(); t_it++ )
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

    link* node::in( const string& p_label )
    {
        link_it t_it = f_in_map.find( p_label );
        if( f_in_map.find( p_label ) == f_in_map.end() )
        {
            throw error() << "node named <" << get_name() << "> has no in named <" << p_label << ">";
            return NULL;
        }
        return t_it->second;
    }
    link* node::out( const string& p_label )
    {
        link_it t_it = f_out_map.find( p_label );
        if( f_out_map.find( p_label ) == f_out_map.end() )
        {
            throw error() << "node named <" << get_name() << "> has no out named <" << p_label << ">";
            return NULL;
        }
        return t_it->second;
    }

}
