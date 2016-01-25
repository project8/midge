#ifndef _midge_midge_hh_
#define _midge_midge_hh_

#include <map>

#include "cancelable.hh"
#include "coremsg.hh"
#include "node.hh"
#include "thread.hh"

#include <vector>

namespace midge
{

    class midge : public cancelable
    {
        public:
            midge();
            ~midge();

        public:
            void add( node* p_node );
            void join( const std::string& p_string );
            void run( const std::string& p_string );

        private:
            virtual void do_cancellation();
            virtual void do_reset_cancellation();

            typedef std::map< std::string, node* > node_map_t;
            typedef node_map_t::iterator node_it_t;
            typedef node_map_t::const_iterator node_cit_t;
            typedef node_map_t::value_type node_entry_t;

            node_map_t f_nodes;

            typedef std::vector< thread* > thread_vector_t;
            typedef thread_vector_t::iterator thread_it_t;
            typedef thread_vector_t::const_iterator thread_cit_t;

            thread_vector_t f_threads;

            static const std::string s_connector;
            static const std::string s_designator;
            static const std::string s_separator;
    };
}



#endif
