#ifndef _midge_line_transformer_hh_
#define _midge_line_transformer_hh_

#include "transformer.hh"
#include "rf_data.hh"
#include "point_data.hh"
#include "line_data.hh"
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

    class line_transformer :
        public _transformer< line_transformer, typelist_1( point_data ), typelist_1( point_data )>
    {
        public:
            line_transformer();
            virtual ~line_transformer();

        public:
            accessible( real_t, ratio_seed )
            accessible( real_t, ratio_low )
            accessible( real_t, ratio_high )
            accessible( real_t, window_seed )
            accessible( real_t, window_low )
            accessible( real_t, window_high )
            accessible( real_t, count )
            accessible( real_t, tolerance )
            accessible( count_t, length )

        protected:
            void initialize();
            void execute();
            void finalize();

            void initialize_line( point_data& p_points, line& p_line );
            void update_line( point_data& p_points, line& p_line );


            typedef list< pointer< point > > point_list;
            typedef point_list::iterator point_it;
            typedef point_list::const_iterator point_cit;

            typedef list< pointer< line > > line_list;
            typedef line_list::iterator line_it;
            typedef line_list::const_iterator line_cit;

            typedef stack< line_it > line_stack;
    };

}

#endif
