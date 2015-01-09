#ifndef _midge_event_hh_
#define _midge_event_hh_

#include "line_data.hh"

#include <list>
using std::list;

namespace midge
{

    class event
    {
        private:
            static count_t s_id;

        public:
            ;accessible_static( real_t, frequency )
            ;accessible_static( real_t, time )
            ;accessible_static( real_t, tolerance )
            ;accessible_static( real_t, delay )
            ;assignable_static( line_data, line_data )

        public:
            typedef list< line > line_list;
            typedef line_list::iterator line_it;
            typedef line_list::const_iterator line_cit;

        public:
            event();
            event( const event& p_copy );
            ~event();

            void initialize( const count_t& p_index );
            bool update();

        public:
            ;referrable( count_t, id )
            ;referrable( real_t, time )
            ;referrable( real_t, frequency )
            ;referrable( real_t, slope )
            ;referrable( line_list, lines )
    };

    template< >
    class ascii::pull< event >
    {
        public:
            pull( ascii& p_stream, event& p_data )
            {
                p_stream >>
                p_data.id() >>
                p_data.time() >>
                p_data.frequency() >>
                p_data.slope() >>
                p_data.lines();
            }
    };
    template< >
    class ascii::push< event >
    {
        public:
            push( ascii& p_stream, const event& p_data )
            {
                p_stream <<
                p_data.id() << " " <<
                p_data.time() << " " <<
                p_data.frequency() << " " <<
                p_data.slope() << '\n' <<
                p_data.lines() << '\n';
            }
    };

    template< >
    class binary::pull< event >
    {
        public:
            pull( binary& p_stream, event& p_data )
            {
                p_stream >>
                p_data.id() >>
                p_data.time() >>
                p_data.frequency() >>
                p_data.slope() >>
                p_data.lines();
            }
    };
    template< >
    class binary::push< event >
    {
        public:
            push( binary& p_stream, const event& p_data )
            {
                p_stream <<
                p_data.id() <<
                p_data.time() <<
                p_data.frequency() <<
                p_data.slope() <<
                p_data.lines();
            }
    };

}

#endif
