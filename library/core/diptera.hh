#ifndef _midge_midge_hh_
#define _midge_midge_hh_

#include "cancelable.hh"
#include "instructable.hh"

#include <map>
#include <set>
#include <vector>

namespace midge
{
    class node;
    class producer;
    class thread;

    class diptera : public cancelable
    {
        public:
            diptera();
            ~diptera();

        public:
            void add( node* p_node );
            void join( const std::string& p_string );
            void run( const std::string& p_string );

            void reset();

            void instruct( instruction p_inst );

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

            typedef std::vector< thread* > thread_vector_t;
            typedef thread_vector_t::iterator thread_it_t;
            typedef thread_vector_t::const_iterator thread_cit_t;

            thread_vector_t f_threads;
            std::mutex f_threads_mutex;

            static const std::string s_connector;
            static const std::string s_designator;
            static const std::string s_separator;
    };
}



#endif
