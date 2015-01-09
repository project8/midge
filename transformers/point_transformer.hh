#ifndef _midge_point_transformer_hh_
#define _midge_point_transformer_hh_

#include "transformer.hh"
#include "rf_data.hh"
#include "rtf_data.hh"
#include "point_data.hh"
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

    class point_transformer :
        public _transformer< point_transformer, typelist_2( rf_data, rtf_data ), typelist_1( point_data )>
    {
        public:
            point_transformer();
            virtual ~point_transformer();

        public:
            accessible( real_t, threshold )
            accessible( real_t, high_factor )
            accessible( real_t, high_power )
            accessible( real_t, low_factor )
            accessible( real_t, low_power )
            accessible( real_t, begin_sec )
            accessible( real_t, end_sec )
            accessible( real_t, begin_hz )
            accessible( real_t, end_hz )
            accessible( count_t, length )

        protected:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
