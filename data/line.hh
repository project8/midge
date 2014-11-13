#ifndef _midge_line_hh_
#define _midge_line_hh_

#include "point.hh"
#include "point_data.hh"

#include <list>
using std::list;

namespace midge
{

    class line
    {
        public:
            ;accessible_static( real_t, window )
            ;accessible_static( real_t, width )
            ;accessible_static( real_t, gap )
            ;accessible_static( count_t, count )

            ;assignable_static( point_data, data )

        public:
            typedef list< point > point_list;
            typedef point_list::iterator point_it;
            typedef point_list::const_iterator point_cit;

        public:
            typedef list< point_list > group_list;
            typedef group_list::iterator group_it;
            typedef group_list::const_iterator group_cit;

        public:
            line();
            line( const line& a_copy );
            ~line();

            void initialize( const count_t& t_index );
            void update();
            void finalize();

        public:
            ;referrable( count_t, id )
            ;referrable( count_t, count )
            ;referrable( real_t, score )
            ;referrable( real_t, quality )
            ;referrable( real_t, time )
            ;referrable( real_t, frequency )
            ;referrable( real_t, slope )
            ;referrable( real_t, duration )
            ;referrable( real_t, correlation )
            ;referrable( real_t, deviation )
            ;referrable( point_list, points )

        public:
            real_t weight( const real_t& p_frequency ) const;

            group_list f_low;
            count_t f_low_count;
            group_list f_high;
            count_t f_high_count;
            real_t f_r_sum;
            real_t f_rt_sum;
            real_t f_rf_sum;
            real_t f_rtt_sum;
            real_t f_rff_sum;
            real_t f_rtf_sum;
    };

}
#endif
