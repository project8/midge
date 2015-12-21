#include "../library/core/node.hh"

#include "../utility/error.hh"

namespace midge
{

    node::node() :
            f_name( "(unnamed node)" ),
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

    input* node::in( const string& p_label )
    {
        input_it t_it = f_input_map.find( p_label );
        if( f_input_map.find( p_label ) == f_input_map.end() )
        {
            throw error() << "node named <" << get_name() << "> has no input named <" << p_label << ">";
            return NULL;
        }
        return t_it->second;
    }
    output* node::out( const string& p_label )
    {
        output_it t_it = f_output_map.find( p_label );
        if( f_output_map.find( p_label ) == f_output_map.end() )
        {
            throw error() << "node named <" << get_name() << "> has no out named <" << p_label << ">";
            return NULL;
        }
        return t_it->second;
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
            throw error() << "node already has in named <" << p_label << ">";
        }
        return;
    }

}
