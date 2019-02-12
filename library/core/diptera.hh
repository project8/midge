#ifndef _midge_midge_hh_
#define _midge_midge_hh_

#include "instructable.hh"

#include "cancelable.hh"

#include <functional>
#include <map>
#include <set>
#include <thread>
#include <vector>

namespace midge
{
    class node;
    class producer;
    class thread;

    class diptera : public scarab::cancelable
    {
        public:
            diptera();
            ~diptera();

        public:
            /// Add a node
            void add( node* p_node );

            ///  Connect a signal to a slot: signal_node.signal:slot_node.slot
            void connect( const std::string& p_string );

            /// Join one node to another
            /*!
             * Option 1: Join by data stream
             *   "my-producer.out_0:my-consumer.in_0"
             *     Output 0 of my-producer will be connected to input 0 of my-consumer
             *
             * Option 2: Join by pointer
             *   "my-consumer:my-producer"
             *     my-consumer will get a pointer to my-producer stored in the f_node_links map
             */
            void join( const std::string& p_string );

            /// Run a set of nodes; only the nodes specified will be executed
            ///   "my-consumer:my-producer"
            std::exception_ptr run( const std::string& p_string );

            /// To be used by running nodes to throw an exception
            void throw_ex( std::exception_ptr e_ptr );

            void reset();

            void instruct( instruction p_inst );

            void set_running_callback( std::function< void() > a_callback );

            static const std::string& connector();
            static const std::string& designator();
            static const std::string& separator();

        private:
            virtual void do_cancellation();
            virtual void do_reset_cancellation();

            typedef std::map< std::string, node* > node_map_t;
            typedef node_map_t::iterator node_it_t;
            typedef node_map_t::const_iterator node_cit_t;
            typedef node_map_t::value_type node_entry_t;

            node_map_t f_nodes;

            typedef std::set< instructable* > inst_set_t;
            typedef inst_set_t::iterator inst_it_t;
            typedef inst_set_t::const_iterator inst_cit_t;
            typedef inst_set_t::value_type inst_entry_t;

            inst_set_t f_instructables;

            typedef std::vector< std::thread > thread_vector_t;
            typedef thread_vector_t::iterator thread_it_t;
            typedef thread_vector_t::const_iterator thread_cit_t;

            thread_vector_t f_threads;
            std::mutex f_threads_mutex;

            std::exception_ptr f_run_e_ptr;

            std::function< void() > f_running_callback;

            static const std::string s_connector;
            static const std::string s_designator;
            static const std::string s_separator;
    };

    inline void diptera::set_running_callback( std::function< void() > a_callback )
    {
        f_running_callback = a_callback;
        return;
    }

}

#endif
