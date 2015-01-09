#ifndef _midge_line_hh_
#define _midge_line_hh_

#include "point_data.hh"

#include <list>
using std::list;

namespace midge
{

    class line
    {
        private:
            static count_t s_id;

        public:
            ;accessible_static( real_t, window )
            ;accessible_static( real_t, width )
            ;accessible_static( real_t, trim )
            ;accessible_static( real_t, block )
            ;accessible_static( count_t, count )
            ;assignable_static( point_data, point_data )

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
            bool update();
            void finalize();

        public:
            ;referrable( bool_t, free )
            ;referrable( count_t, id )
            ;referrable( real_t, time )
            ;referrable( real_t, frequency )
            ;referrable( real_t, slope )
            ;referrable( real_t, duration )
            ;referrable( real_t, correlation )
            ;referrable( real_t, deviation )
            ;referrable( real_t, score )
            ;referrable( real_t, quality )
            ;referrable( point_list, points )

        private:
            group_list f_cluster;
            count_t f_cluster_count;

            group_list f_line;
            count_t f_line_count;

            real_t f_r_sum;
            real_t f_rt_sum;
            real_t f_rf_sum;
            real_t f_rtt_sum;
            real_t f_rff_sum;
            real_t f_rtf_sum;
    };

    template< >
    class ascii::pull< line >
    {
        public:
            pull( ascii& p_stream, line& p_data )
            {
                p_stream >>
                p_data.free() >>
                p_data.id() >>
                p_data.time() >>
                p_data.frequency() >>
                p_data.slope() >>
                p_data.duration() >>
                p_data.deviation() >>
                p_data.correlation() >>
                p_data.score() >>
                p_data.quality() >>
                p_data.points();
            }
    };
    template< >
    class ascii::push< line >
    {
        public:
            push( ascii& p_stream, const line& p_data )
            {
                p_stream <<
                p_data.free() << " " <<
                p_data.id() << " " <<
                p_data.time() << " " <<
                p_data.frequency() << " " <<
                p_data.slope() << " " <<
                p_data.duration() << " " <<
                p_data.deviation() << " " <<
                p_data.correlation() << " " <<
                p_data.score() << " " <<
                p_data.quality() << '\n' <<
                p_data.points() << '\n';
            }
    };

    template< >
    class binary::pull< line >
    {
        public:
            pull( binary& p_stream, line& p_data )
            {
                p_stream >>
                p_data.free() >>
                p_data.id() >>
                p_data.time() >>
                p_data.frequency() >>
                p_data.slope() >>
                p_data.duration() >>
                p_data.deviation() >>
                p_data.correlation() >>
                p_data.score() >>
                p_data.quality() >>
                p_data.points();
            }
    };
    template< >
    class binary::push< line >
    {
        public:
            push( binary& p_stream, const line& p_data )
            {
                p_stream <<
                p_data.free() <<
                p_data.id() <<
                p_data.time() <<
                p_data.frequency() <<
                p_data.slope() <<
                p_data.duration() <<
                p_data.deviation() <<
                p_data.correlation() <<
                p_data.score() <<
                p_data.quality() <<
                p_data.points();
            }
    };

}
#endif
