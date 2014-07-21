#include "root.hh"

namespace midge
{

    root::root() :
            f_map()
    {
    }
    root::~root()
    {
        for( map_it_t t_it = f_map.begin(); t_it != f_map.end(); t_it++ )
        {
            delete (t_it->second);
        }
    }

    void root::add( node* p_node )
    {
        string t_name = p_node->get_name();
        map_it_t t_it = f_map.find( t_name );
        if( t_it == f_map.end() )
        {
            f_map.insert( map_entry_t( t_name, p_node ) );
        }
        else
        {
            throw error() << "root add found preexisting node with name <" << t_name << ">";
        }
        return;
    }
    void root::join( const string& p_string )
    {
        string t_first_node_string( "" );
        node* t_first_node;
        string t_first_output_string( "" );
        link* t_first_output;
        string t_second_node_string( "" );
        node* t_second_node;
        string t_second_input_string( "" );
        link* t_second_input;

        size_t t_first_pos;
        size_t t_second_pos;
        string t_first_argument;
        string t_second_argument;

        t_first_pos = p_string.find( s_separator );
        if( t_first_pos != string::npos )
        {
            t_second_pos = p_string.find( s_separator, t_first_pos + s_separator.length() );
            if( t_second_pos == string::npos )
            {
                t_first_argument = p_string.substr( 0, t_first_pos );
                t_second_argument = p_string.substr( t_first_pos + 1, string::npos );

                t_first_pos = t_first_argument.find( s_designator );
                if( t_first_pos != string::npos )
                {
                    t_second_pos = t_first_argument.find( s_designator, t_first_pos + s_designator.length() );
                    if( t_second_pos == string::npos )
                    {
                        t_first_node_string = t_first_argument.substr( 0, t_first_pos );
                        t_first_output_string = t_first_argument.substr( t_first_pos + 1, string::npos );
                    }
                    else
                    {
                        throw error() << "root join found multiple designators in first argument <" << t_first_argument << ">";
                        return;
                    }
                }
                else
                {
                    t_first_node_string = t_first_argument;
                }

                t_first_pos = t_second_argument.find( s_designator );
                if( t_first_pos != string::npos )
                {
                    t_second_pos = t_second_argument.find( s_designator, t_first_pos + s_designator.length() );
                    if( t_second_pos == string::npos )
                    {
                        t_second_node_string = t_second_argument.substr( 0, t_first_pos );
                        t_second_input_string = t_second_argument.substr( t_first_pos + 1, string::npos );
                    }
                    else
                    {
                        throw error() << "root join found multiple designators in second argument <" << t_second_argument << ">";
                        return;
                    }
                }
                else
                {
                    t_second_node_string = t_second_argument;
                }

                if( (t_first_output_string.length() > 0) && (t_second_input_string.length() > 0) )
                {
                    map_it_t t_first_it = f_map.find( t_first_node_string );
                    if( t_first_it == f_map.end() )
                    {
                        throw error() << "root join found no first node with name <" << t_first_node_string << ">";
                        return;
                    }
                    t_first_node = t_first_it->second;

                    map_it_t t_second_it = f_map.find( t_second_node_string );
                    if( t_second_it == f_map.end() )
                    {
                        throw error() << "root join found no second node with name <" << t_second_node_string << ">";
                        return;
                    }
                    t_second_node = t_second_it->second;

                    t_first_output = t_first_node->output( t_first_output_string );
                    if( t_first_output == NULL )
                    {
                        throw error() << "root join found no first output with name <" << t_first_output_string << "> in node with name <" << t_first_node_string << ">";
                        return;
                    }
                    t_first_output->connect( t_second_node );

                    t_second_input = t_second_node->input( t_second_input_string );
                    if( t_second_input == NULL )
                    {
                        throw error() << "root join found no second input with name <" << t_second_input_string << "> in node with name <" << t_second_node_string << ">";
                        return;
                    }
                    t_second_input->connect( t_first_node );

                    return;
                }

                if( t_first_output_string.length() > 0 )
                {
                    map_it_t t_first_it = f_map.find( t_first_node_string );
                    if( t_first_it == f_map.end() )
                    {
                        throw error() << "root join found no first node with name <" << t_first_node_string << ">";
                        return;
                    }
                    t_first_node = t_first_it->second;

                    map_it_t t_second_it = f_map.find( t_second_node_string );
                    if( t_second_it == f_map.end() )
                    {
                        throw error() << "root join found no second node with name <" << t_second_node_string << ">";
                        return;
                    }
                    t_second_node = t_second_it->second;

                    t_first_output = t_first_node->output( t_first_output_string );
                    if( t_first_output == NULL )
                    {
                        throw error() << "root join found no first output with name <" << t_first_output_string << "> in node with name <" << t_first_node_string << ">";
                        return;
                    }
                    t_first_output->connect( t_second_node );

                    return;
                }

                if( t_second_input_string.length() > 0 )
                {
                    map_it_t t_first_it = f_map.find( t_first_node_string );
                    if( t_first_it == f_map.end() )
                    {
                        throw error() << "root join found no first node with name <" << t_first_node_string << ">";
                        return;
                    }
                    t_first_node = t_first_it->second;

                    map_it_t t_second_it = f_map.find( t_second_node_string );
                    if( t_second_it == f_map.end() )
                    {
                        throw error() << "root join found no second node with name <" << t_second_node_string << ">";
                        return;
                    }
                    t_second_node = t_second_it->second;

                    t_second_input = t_second_node->output( t_second_input_string );
                    if( t_second_input == NULL )
                    {
                        throw error() << "root join found no second input with name <" << t_second_input_string << "> in node with name <" << t_second_node_string << ">";
                        return;
                    }
                    t_second_input->connect( t_first_node );

                    return;
                }

                throw error() << "root join found no links in string <" << p_string << ">";
                return;
            }
            else
            {
                throw error() << "root join found multiple separators in string <" << p_string << ">";
                return;
            }
        }
        else
        {
            throw error() << "root join found no separator in string <" << p_string << ">";
            return;
        }
    }
    void root::run( const string& p_string )
    {
        map_it_t t_it = f_map.find( p_string );
        if( t_it != f_map.end() )
        {
            node* t_node = t_it->second;

            t_node->initialize();
            t_node->execute();
            t_node->finalize();
        }
        else
        {
            throw error() << "root run found no node with name <" << p_string << ">";
        }
        return;
    }

    const string root::s_separator = string( ":" );
    const string root::s_designator = string( "." );

}
