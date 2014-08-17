#ifndef _midge_root_hh_
#define _midge_root_hh_

#include "coremsg.hh"

#include "node.hh"
#include "thread.hh"

#include <map>
using std::map;

#include <vector>
using std::vector;

namespace midge
{

    class root
    {
        public:
            root();
            ~root();

        public:
            void add( node* p_node );
            void join( const string& p_string );
            void run( const string& p_string );

        private:
            typedef map< string, node* > node_map_t;
            typedef node_map_t::iterator node_it_t;
            typedef node_map_t::const_iterator node_cit_t;
            typedef node_map_t::value_type node_entry_t;

            node_map_t f_nodes;

            typedef vector< thread* > thread_vector_t;
            typedef thread_vector_t::iterator thread_it_t;
            typedef thread_vector_t::const_iterator thread_cit_t;

            thread_vector_t f_threads;

            static const string s_connector;
            static const string s_designator;
            static const string s_separator;
    };
}



#endif
