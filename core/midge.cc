#include "midge.hh"

#include "input.hh"
#include "output.hh"
#include "error.hh"

#include "coremsg.hh"

#include <unistd.h>

namespace midge
{

    midge::midge() :
            f_nodes()
    {
    }
    midge::~midge()
    {
        node* t_node;
        node_it_t t_it;
        for( t_it = f_nodes.begin(); t_it != f_nodes.end(); t_it++ )
        {
            t_node = t_it->second;
            t_node->finalize();
        }
        for( t_it = f_nodes.begin(); t_it != f_nodes.end(); t_it++ )
        {
            t_node = t_it->second;
            delete (t_node);
        }
    }

    void midge::add( node* p_node )
    {
        string_t t_name = p_node->get_name();
        node_it_t t_it = f_nodes.find( t_name );
        if( t_it == f_nodes.end() )
        {
            p_node->initialize();
            f_nodes.insert( node_entry_t( t_name, p_node ) );
            msg_normal( coremsg, "added node <" << t_name << ">" << eom );
        }
        else
        {
            throw error() << "root add found preexisting node with name <" << t_name << ">";
        }
        return;
    }
    void midge::join( const string_t& p_string )
    {
        string_t t_first_node_string( "" );
        node* t_first_node;
        string_t t_first_out_string( "" );
        output* t_first_out;
        string_t t_second_node_string( "" );
        node* t_second_node;
        string_t t_second_in_string( "" );
        input* t_second_in;

        size_t t_first_pos;
        size_t t_second_pos;
        string_t t_first_argument;
        string_t t_second_argument;

        t_first_pos = p_string.find( s_connector );
        if( t_first_pos != string_t::npos )
        {
            t_second_pos = p_string.find( s_connector, t_first_pos + s_connector.length() );
            if( t_second_pos == string_t::npos )
            {
                t_first_argument = p_string.substr( 0, t_first_pos );
                t_second_argument = p_string.substr( t_first_pos + 1, string_t::npos );

                t_first_pos = t_first_argument.find( s_designator );
                if( t_first_pos != string_t::npos )
                {
                    t_second_pos = t_first_argument.find( s_designator, t_first_pos + s_designator.length() );
                    if( t_second_pos == string_t::npos )
                    {
                        t_first_node_string = t_first_argument.substr( 0, t_first_pos );
                        t_first_out_string = t_first_argument.substr( t_first_pos + 1, string_t::npos );
                    }
                    else
                    {
                        throw error() << "root join found multiple designators in first argument <" << t_first_argument << ">";
                        return;
                    }
                }
                else
                {
                    throw error() << "root join found no designators in first argument <" << t_first_argument << ">";
                    return;
                }

                t_first_pos = t_second_argument.find( s_designator );
                if( t_first_pos != string_t::npos )
                {
                    t_second_pos = t_second_argument.find( s_designator, t_first_pos + s_designator.length() );
                    if( t_second_pos == string_t::npos )
                    {
                        t_second_node_string = t_second_argument.substr( 0, t_first_pos );
                        t_second_in_string = t_second_argument.substr( t_first_pos + 1, string_t::npos );
                    }
                    else
                    {
                        throw error() << "root join found multiple designators in second argument <" << t_second_argument << ">";
                        return;
                    }
                }
                else
                {
                    throw error() << "root join found no designators in second argument <" << t_second_argument << ">";
                    return;
                }

                node_it_t t_first_it = f_nodes.find( t_first_node_string );
                if( t_first_it == f_nodes.end() )
                {
                    throw error() << "root join found no first node with name <" << t_first_node_string << ">";
                    return;
                }
                t_first_node = t_first_it->second;

                node_it_t t_second_it = f_nodes.find( t_second_node_string );
                if( t_second_it == f_nodes.end() )
                {
                    throw error() << "root join found no second node with name <" << t_second_node_string << ">";
                    return;
                }
                t_second_node = t_second_it->second;

                t_first_out = t_first_node->out( t_first_out_string );
                if( t_first_out == NULL )
                {
                    throw error() << "root join found no first out with name <" << t_first_out_string << "> in node with name <" << t_first_node_string << ">";
                    return;
                }

                t_second_in = t_second_node->in( t_second_in_string );
                if( t_second_in == NULL )
                {
                    throw error() << "root join found no second in with name <" << t_second_in_string << "> in node with name <" << t_second_node_string << ">";
                    return;
                }
                t_second_in->set( t_first_out->get() );

                msg_normal( coremsg, "joined <" << t_first_node_string << "." << t_first_out_string << "> with <" << t_second_node_string << "." << t_second_in_string << ">" << eom );

                return;
            }
            else
            {
                throw error() << "root join found multiple connectors in string_t <" << p_string << ">";
                return;
            }
        }
        else
        {
            throw error() << "root join found no connector in string_t <" << p_string << ">";
            return;
        }
    }
    void midge::run( const string_t& p_string )
    {
        size_t t_start_pos;
        size_t t_separator_pos;
        string t_argument;
        string t_node_name;
        node_it_t t_node_it;
        node* t_node;
        thread* t_thread;

        t_start_pos = 0;
        t_argument = p_string;
        while( true )
        {
            t_separator_pos = t_argument.find( s_separator, t_start_pos );

            t_node_name = t_argument.substr( t_start_pos, t_separator_pos - t_start_pos );
            t_argument = t_argument.substr( t_separator_pos + s_separator.size(), string_t::npos );

            if( t_node_name.size() == 0 )
            {
                throw error() << "root run found node name with length zero in argument <" << p_string << ">";
                return;
            }

            t_node_it = f_nodes.find( t_node_name );
            if( t_node_it == f_nodes.end() )
            {
                throw error() << "root run found no node with name <" << t_node_name << ">";
            }

            msg_normal( coremsg, "creating thread for node <" << t_node_name << ">" << eom );
            t_node = t_node_it->second;
            t_thread = new thread();
            t_thread->start( t_node, &node::execute );
            f_threads.push_back( t_thread );

            if( t_separator_pos == string_t::npos )
            {
                break;
            }
        }

        msg_normal( coremsg, "starting threads..." << eom );
        for( thread_it_t t_it = f_threads.begin(); t_it != f_threads.end(); t_it++ )
        {
            (*t_it)->start();
        }

        usleep( 100000 );

        msg_normal( coremsg, "joining threads..." << eom );
        for( thread_it_t t_it = f_threads.begin(); t_it != f_threads.end(); t_it++ )
        {
            (*t_it)->join();
        }

        msg_normal( coremsg, "...done" << eom );
        for( thread_it_t t_it = f_threads.begin(); t_it != f_threads.end(); t_it++ )
        {
            delete (*t_it);
        }
        f_threads.clear();

        return;
    }

    const string_t midge::s_connector = string_t( ":" );
    const string_t midge::s_designator = string_t( "." );
    const string_t midge::s_separator = string_t( ":" );

}
