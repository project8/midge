#ifndef _midge_event_transformer_hh_
#define _midge_event_transformer_hh_

#include "transformer.hh"
#include "line_data.hh"
#include "event_data.hh"
#include "typelist.hh"
#include "macros.hh"

#include <set>
using std::multiset;

#include <list>
using std::list;

#include <stack>
using std::stack;

namespace midge
{

    class event_transformer :
        public _transformer< event_transformer, typelist_1( line_data ), typelist_1( event_data )>
    {
        public:
            event_transformer();
            virtual ~event_transformer();

        public:
            accessible( real_t, frequency )
            accessible( real_t, time )
            accessible( real_t, tolerance )
            accessible( real_t, delay )
            accessible( count_t, length )

        protected:
            void initialize();
            void execute();
            void finalize();

        protected:
            typedef list< event* > event_list;
            typedef event_list::iterator event_it;
            typedef event_list::const_iterator event_cit;

            typedef stack< event_it > event_stack;
    };

}

#endif
