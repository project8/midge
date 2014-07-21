#ifndef _midge_root_hh_
#define _midge_root_hh_

#include "node.hh"
#include "link.hh"

#include <map>
using std::map;

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
            typedef map< string, node* > map_t;
            typedef map_t::iterator map_it_t;
            typedef map_t::const_iterator map_cit_t;
            typedef map_t::value_type map_entry_t;

            map_t f_map;

            static const string s_separator;
            static const string s_designator;
    };
}



#endif
