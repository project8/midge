#include "input.hh"

#include <unistd.h>

#include "midge_error.hh"
#include "bystander.hh"
#include "consumer.hh"
#include "coremsg.hh"
#include "diptera.hh"
#include "input.hh"
#include "node.hh"
#include "output.hh"
#include "producer.hh"
#include "transformer.hh"

#include <chrono>
#include <thread>

using std::string;

namespace midge
{

    diptera::diptera() :
            scarab::cancelable(),
            f_nodes(),
            f_instructables(),
            f_threads(),
            f_threads_mutex(),
            f_run_e_ptr(),
            f_running_callback( [](){ return; } )
    {
    }
    diptera::~diptera()
    {
        reset();
    }

    void diptera::add( node* p_node )
    {
        string_t t_name = p_node->get_name();
        node_it_t t_it = f_nodes.find( t_name );
        if( t_it == f_nodes.end() )
        {
            msg_normal( coremsg, "initializing node <" << t_name << ">" << eom );
            try
            {
                p_node->initialize();
            }
            catch( std::exception& e )
            {
                msg_error( coremsg, "exception caught while initializing node <" << t_name << ">: " << e.what() << eom );
                throw( e );
            }

            f_nodes.insert( node_entry_t( t_name, p_node ) );
            msg_normal( coremsg, "added node <" << t_name << ">" << eom );

            instructable* t_inst = dynamic_cast< instructable* >( p_node );
            if( t_inst != nullptr )
            {
                f_instructables.insert( t_inst );
            }
        }
        else
        {
            throw error() << "root add found preexisting node with name <" << t_name << ">";
        }
        return;
    }

    void diptera::connect( const std::string& p_string )
    {
        size_t t_pos = p_string.find( s_connector );
        if( t_pos <= 1 ||
                t_pos >= p_string.length()-2 ||
            p_string.find( s_connector, t_pos + s_connector.length() ) != string_t::npos )
        {
            throw error() << "connection specification <" << p_string << "> was not formatted correctly: signal_node.signal:slot_node.slot";
            return;
        }

        string_t t_signal_argument = p_string.substr( 0, t_pos );
        string_t t_slot_argument = p_string.substr( t_pos + s_connector.length(), string_t::npos );

        t_pos = t_signal_argument.find( s_designator );
        if( t_pos == 0 ||
            t_pos == string::npos ||
            t_signal_argument.find( s_designator, t_pos + s_designator.length() ) != string_t::npos )
        {
            throw error() << "signal specification <" << t_signal_argument << "> is not formatted correctly: signal_node.signal";
            return;
        }

        string_t t_signal_node_string = t_signal_argument.substr( 0, t_pos );
        string_t t_signal_string = t_signal_argument.substr( t_pos + s_designator.length(), string_t::npos );

        t_pos = t_slot_argument.find( s_designator );
        if( t_pos == 0 ||
            t_pos == string::npos ||
            t_slot_argument.find( s_designator, t_pos + s_designator.length() ) != string_t::npos )
        {
            throw error() << "slot specification <" << t_slot_argument << "> is not formatted correctly: slot_node.slot";
            return;
        }

        string_t t_slot_node_string = t_slot_argument.substr( 0, t_pos );
        string_t t_slot_string = t_slot_argument.substr( t_pos + s_designator.length(), string_t::npos );

        node_it_t t_node_it = f_nodes.find( t_signal_node_string );
        if( t_node_it == f_nodes.end() )
        {
            throw error() << "signal node was not found <" << t_signal_node_string << ">";
            return;
        }
        node* t_signal_node = t_node_it->second;

        t_node_it = f_nodes.find( t_slot_node_string );
        if( t_node_it == f_nodes.end() )
        {
            throw error() << "slot node was not found <" << t_slot_node_string << ">";
            return;
        }
        node* t_slot_node = t_node_it->second;

        signal* t_signal = t_signal_node->signal_ptr( t_signal_string );
        if( t_signal == NULL )
        {
            throw error() << "signal <" << t_signal_string << "> was not found for node <" << t_signal_node << ">";
            return;
        }

        slot* t_slot = t_slot_node->slot_ptr( t_slot_string );
        if( t_slot == NULL )
        {
            throw error() << "slot <" << t_slot_string << "> was not found for node <" << t_slot_node_string << ">";
            return;
        }

        t_signal->connect( t_slot );

        msg_normal( coremsg, "connected signal --> slot: " << t_signal_node_string << "." << t_signal_string << " --> " << t_slot_node_string << "." << t_slot_string << eom );

        return;
    }

    void diptera::join( const string_t& p_string )
    {
        string_t t_first_node_string;
        node* t_first_node;
        string_t t_first_out_string;
        output* t_first_out;
        string_t t_second_node_string;
        node* t_second_node;
        string_t t_second_in_string;
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

                if( t_first_out_string.empty() != t_second_in_string.empty() )
                {
                    throw error() << "root join must link either by stream or pointer";
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

                if( t_first_out_string.empty() )
                {
                    // joining nodes by pointer
                    t_first_node->node_ptr( t_second_node, t_second_node_string );

                    msg_normal( coremsg, "joined <" << t_first_node_string << "> with <" << t_second_node_string << ">" << eom );
                }
                else
                {

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
                }

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

    std::exception_ptr diptera::run( const string_t& p_string )
    {
        size_t t_start_pos;
        size_t t_separator_pos;
        string t_argument;
        string t_node_name;
        node_it_t t_node_it;
        node* t_node;

        t_start_pos = 0;
        t_argument = p_string;

        std::unique_lock< std::mutex >t_threads_lock( f_threads_mutex );

        // add nodes to the instructables map
        for( node_cit_t t_it = f_nodes.begin(); t_it != f_nodes.end(); ++t_it )
        {
            instructable* t_inst = dynamic_cast< instructable* >( t_it->second );
            if( t_inst != nullptr )
            {
                f_instructables.insert( t_inst );
            }
        }

        // run nodes specified in the string
        while( true )
        {
            t_separator_pos = t_argument.find( s_separator, t_start_pos );

            t_node_name = t_argument.substr( t_start_pos, t_separator_pos - t_start_pos );
            t_argument = t_argument.substr( t_separator_pos + s_separator.size(), string_t::npos );

            if( t_node_name.size() == 0 )
            {
                return std::make_exception_ptr( error() << "root run found node name with length zero in argument <" << p_string << ">" );
            }

            t_node_it = f_nodes.find( t_node_name );
            if( t_node_it == f_nodes.end() )
            {
                return std::make_exception_ptr( error() << "root run found no node with name <" << t_node_name << ">" );
            }

            msg_normal( coremsg, "creating thread for node <" << t_node_name << ">" << eom );
            t_node = t_node_it->second;
            f_threads.push_back( std::thread( &node::execute, t_node, this ) );

            if( t_separator_pos == string_t::npos )
            {
                break;
            }
        }

        // delay to allow the threads to spin up
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

        f_running_callback();

        t_threads_lock.unlock();

        msg_normal( coremsg, "waiting for threads to finish..." << eom );
        for( thread_it_t t_it = f_threads.begin(); t_it != f_threads.end(); t_it++ )
        {
            t_it->join();
        }

        t_threads_lock.lock();

        msg_normal( coremsg, "threads finished" << eom );

        f_threads.clear();

        // clear instructables set
        f_instructables.clear();

        return f_run_e_ptr;
    }

    void diptera::throw_ex( std::exception_ptr e_ptr )
    {
        try
        {
            if( e_ptr )
            {
                std::rethrow_exception( e_ptr );
            }
        }
        catch( const midge::error& e )
        {
            msg_error( coremsg, "midge error thrown: " << e.what() << eom );
        }
        catch( const midge::node_fatal_error& e )
        {
            msg_error( coremsg, "fatal error thrown: " << e.what() << eom );
        }
        catch( const midge::node_nonfatal_error& e )
        {
            msg_error( coremsg, "non-fatal error thrown: " << e.what() << eom );
        }
        catch( const std::exception& e )
        {
            msg_error( coremsg, "non-node exception thrown: " << e.what() << eom );
        }
        msg_debug( coremsg, "canceling run and setting exception pointer" << eom );
        cancel();
        f_run_e_ptr = e_ptr;
        return;
    }


    void diptera::reset()
    {
        std::unique_lock< std::mutex >t_threads_lock( f_threads_mutex );

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
        f_nodes.clear();

        f_instructables.clear();

        return;
    }

    void diptera::instruct( instruction p_inst )
    {
        std::unique_lock< std::mutex >t_threads_lock( f_threads_mutex );

        for( inst_it_t t_it = f_instructables.begin(); t_it != f_instructables.end(); ++t_it )
        {
            (*t_it)->instruct( p_inst );
        }

        return;
    }

    void diptera::do_cancellation( int a_code )
    {
        std::unique_lock< std::mutex >t_threads_lock( f_threads_mutex );

        // cancel producers first
        msg_debug( coremsg, "Canceling nodes: producers" << eom );
        for( node_it_t t_it = f_nodes.begin(); t_it != f_nodes.end(); t_it++ )
        {
            if( dynamic_cast< producer* >( t_it->second ) != nullptr )
            {
                msg_debug( coremsg, "Canceling " << t_it->second->get_name() << eom );
                t_it->second->cancel( a_code );
            }
        }

        // This delay is added to give the producers a chance to stop the chain(s) of nodes.
        // Without this, shutting down midge is somewhat unstable and has resulted in deadlocked threads.
        std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

        // cancel transformers second
        msg_debug( coremsg, "Canceling nodes: transformers" << eom );
        for( node_it_t t_it = f_nodes.begin(); t_it != f_nodes.end(); t_it++ )
        {
            if( dynamic_cast< transformer* >( t_it->second ) != nullptr )
            {
                msg_debug( coremsg, "Canceling " << t_it->second->get_name() << eom );
                t_it->second->cancel();
            }
        }
        // cancel consumers third
        msg_debug( coremsg, "Canceling nodes: consumers" << eom );
        for( node_it_t t_it = f_nodes.begin(); t_it != f_nodes.end(); t_it++ )
        {
            if( dynamic_cast< consumer* >( t_it->second ) != nullptr )
            {
                msg_debug( coremsg, "Canceling " << t_it->second->get_name() << eom );
                t_it->second->cancel();
            }
        }
        // cancel bystanders fourth
        msg_debug( coremsg, "Canceling nodes: bystanders" << eom );
        for( node_it_t t_it = f_nodes.begin(); t_it != f_nodes.end(); t_it++ )
        {
            if( dynamic_cast< bystander* >( t_it->second ) != nullptr )
            {
                msg_debug( coremsg, "Canceling " << t_it->second->get_name() << eom );
                t_it->second->cancel();
            }
        }
        return;
    }

    void diptera::do_reset_cancellation()
    {
        std::unique_lock< std::mutex >t_threads_lock( f_threads_mutex );

        for( node_it_t t_it = f_nodes.begin(); t_it != f_nodes.end(); t_it++ )
        {
            t_it->second->reset_cancel();
        }

        return;
    }


    const string_t diptera::s_connector = string_t( ":" );
    const string_t diptera::s_designator = string_t( "." );
    const string_t diptera::s_separator = string_t( ":" );

    const std::string& diptera::connector()
    {
        return s_connector;
    }

    const std::string& diptera::designator()
    {
        return s_designator;
    }

    const std::string& diptera::separator()
    {
        return s_separator;
    }

}
