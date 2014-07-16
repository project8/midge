#include "component.hh"

namespace midge
{

    component::component() :
            f_input_map(),
            f_output_map()
    {
    }
    component::~component()
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

    link* component::input( const string& p_label )
    {
        link_it t_it = f_input_map.find( p_label );
        if( f_input_map.find( p_label ) == f_input_map.end() )
        {
            //todo: throw here
            return NULL;
        }
        return t_it->second;
    }
    link* component::output( const string& p_label )
    {
        link_it t_it = f_output_map.find( p_label );
        if( f_output_map.find( p_label ) == f_output_map.end() )
        {
            //todo: throw here
            return NULL;
        }
        return t_it->second;
    }

}
