#ifndef _midge_line_hh_
#define _midge_line_hh_

#include "point.hh"
#include "point_data.hh"
#include "pointer.hh"
#include "pool.hh"

#include <list>
using std::list;

namespace midge
{

    class line
    {
        public:
            ;accessible_static( real_t, threshold )
            ;accessible_static( real_t, window )
            ;accessible_static( real_t, width )
            ;accessible_static( count_t, count )
            ;accessible_static( real_t, hit_factor )
            ;accessible_static( real_t, hit_power )
            ;accessible_static( real_t, miss_factor )
            ;accessible_static( real_t, miss_power )

            ;assignable_static( point_data, data )

        public:
            typedef list< pointer< point > > point_list;
            typedef point_list::iterator point_it;

        public:
            typedef list< point_list > group_list;
            typedef group_list::iterator group_it;

        public:
            line();
            ~line();

            void initialize( const count_t& t_index );
            void update();
            void finalize();
            void print();

        public:
            ;referrable( count_t, id )
            ;referrable( real_t, score )
            ;referrable( real_t, quality )
            ;referrable( real_t, time )
            ;referrable( real_t, frequency )
            ;referrable( real_t, slope )
            ;referrable( real_t, duration )
            ;referrable( real_t, correlation )
            ;referrable( real_t, deviation )
            ;referrable( group_list, low )
            ;referrable( group_list, high )
            ;referrable( point_list, global )

        private:
            real_t weight( const real_t& p_frequency ) const;

            count_t f_low_count;
            count_t f_high_count;
            real_t f_local_r_sum;
            real_t f_local_rt_sum;
            real_t f_local_rf_sum;
            real_t f_local_rtt_sum;
            real_t f_local_rff_sum;
            real_t f_local_rtf_sum;

            count_t f_global_count;
            real_t f_global_r_sum;
            real_t f_global_rt_sum;
            real_t f_global_rf_sum;
            real_t f_global_rtt_sum;
            real_t f_global_rff_sum;
            real_t f_global_rtf_sum;

            real_t f_total_score;
    };

}
#endif
