#include "node.hh"

#include "coremsg.hh"
#include "midge_error.hh"

using std::string;

namespace midge
{

    node::node() :
            scarab::cancelable(),
            f_name( "(unnamed node)" ),
            f_node_map(),
            f_input_map(),
            f_output_map()
    {
    }
    node::~node()
    {
        for( input_it t_input_it = f_input_map.begin(); t_input_it != f_input_map.end(); t_input_it++ )
        {
            delete (t_input_it->second);
        }

        for( output_it t_output_it = f_output_map.begin(); t_output_it != f_output_map.end(); t_output_it++ )
        {
            delete (t_output_it->second);
        }
    }

    node* node::node_ptr( const string& p_label )
    {
        node_it t_it = f_node_map.find( p_label );
        if( t_it == f_node_map.end() )
        {
            throw error() << "node named <" << get_name() << "> has no pointer to node named <" << p_label << ">";
            return NULL;
        }
        return t_it->second;
    }
    input* node::in( const string& p_label )
    {
        input_it t_it = f_input_map.find( p_label );
        if( t_it == f_input_map.end() )
        {
            throw error() << "node named <" << get_name() << "> has no input named <" << p_label << ">";
            return NULL;
        }
        return t_it->second;
    }
    output* node::out( const string& p_label )
    {
        output_it t_it = f_output_map.find( p_label );
        if( t_it == f_output_map.end() )
        {
            throw error() << "node named <" << get_name() << "> has no out named <" << p_label << ">";
            return NULL;
        }
        return t_it->second;
    }

    void node::node_ptr( node* p_node, const string& p_label )
    {
        node_it t_it = f_node_map.find( p_label );
        if( t_it == f_node_map.end() )
        {
            f_node_map.insert( node_entry( p_label, p_node ) );
        }
        else
        {
            throw error() << "node already has a node pointer named <" << p_label << ">";
        }
        return;
    }
    void node::in( input* p_input, const string& p_label )
    {
        input_it t_it = f_input_map.find( p_label );
        if( t_it == f_input_map.end() )
        {
            p_input->set_name( p_label );
            f_input_map.insert( input_entry( p_label, p_input ) );
        }
        else
        {
            throw error() << "node already has in named <" << p_label << ">";
        }
        return;
    }
    void node::out( output* p_output, const string& p_label )
    {
        output_it t_it = f_output_map.find( p_label );
        if( t_it == f_output_map.end() )
        {
            p_output->set_name( p_label );
            f_output_map.insert( output_entry( p_label, p_output ) );
        }
        else
        {
            throw error() << "node already has out named <" << p_label << ">";
        }
        return;
    }

    void node::do_cancellation()
    {
        for( output_it t_it = f_output_map.begin(); t_it != f_output_map.end(); ++t_it )
        {
            msg_debug( coremsg, "Canceling stream <" << t_it->first << ">" << eom );
            t_it->second->get()->cancel();
        }
        return;
    }


}
