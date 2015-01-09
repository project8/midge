#ifndef _midge_point_hh_
#define _midge_point_hh_

#include "types.hh"
#include "macros.hh"
#include "ascii.hh"
#include "binary.hh"
#include "rf_data.hh"
#include "rtf_data.hh"

namespace midge
{

    class point
    {
        private:
            static count_t s_id;

        public:
            ;accessible_static( real_t, threshold )
            ;accessible_static( real_t, high_factor )
            ;accessible_static( real_t, high_power )
            ;accessible_static( real_t, low_factor )
            ;accessible_static( real_t, low_power )
            ;assignable_static( rf_data, background_data );
            ;assignable_static( rtf_data, signal_data );

        public:
            point();
            point( const point& p_copy );
            ~point();

            void initialize( const count_t& p_index );

        public:
            ;referrable( bool_t, free )
            ;referrable( count_t, id )
            ;referrable( real_t, time )
            ;referrable( real_t, frequency )
            ;referrable( real_t, ratio )
            ;referrable( real_t, score )
    };

    template< >
    class ascii::pull< point >
    {
        public:
            pull( ascii& p_stream, point& p_data )
            {
                p_stream >>
                p_data.free() >>
                p_data.id() >>
                p_data.time() >>
                p_data.frequency() >>
                p_data.ratio() >>
                p_data.score();
            }
    };
    template< >
    class ascii::push< point >
    {
        public:
            push( ascii& p_stream, const point& p_data )
            {
                p_stream <<
                p_data.free() << " " <<
                p_data.id() << " " <<
                p_data.time() << " " <<
                p_data.frequency() << " " <<
                p_data.ratio() << " " <<
                p_data.score() << '\n';
            }
    };

    template< >
    class binary::pull< point >
    {
        public:
            pull( binary& p_stream, point& p_data )
            {
                p_stream >>
                p_data.free() >>
                p_data.id() >>
                p_data.time() >>
                p_data.frequency() >>
                p_data.ratio() >>
                p_data.score();
            }
    };
    template< >
    class binary::push< point >
    {
        public:
            push( binary& p_stream, const point& p_data )
            {
                p_stream <<
                p_data.free() <<
                p_data.id() <<
                p_data.time() <<
                p_data.frequency() <<
                p_data.ratio() <<
                p_data.score();
            }
    };

}

#endif
