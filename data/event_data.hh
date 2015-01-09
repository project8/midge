#ifndef _midge_event_data_hh_
#define _midge_event_data_hh_

#include "event.hh"

#include <vector>
using std::vector;

namespace midge
{

    class event_data
    {
        public:
            event_data();
            virtual ~event_data();

        public:
            ;referrable( vector< event >, events )
            ;referrable( count_t, size )
            ;referrable( real_t, frequency_interval )
            ;referrable( count_t, frequency_index )
            ;referrable( real_t, time_interval )
            ;referrable( count_t, time_index )
    };

    template< >
    class ascii::pull< event_data >
    {
        public:
            pull( ascii& p_stream, event_data& p_data )
            {
                p_stream >>
                p_data.size() >>
                p_data.frequency_interval() >>
                p_data.frequency_index() >>
                p_data.time_interval() >>
                p_data.time_index() >>
                p_data.events();
            }
    };
    template< >
    class ascii::push< event_data >
    {
        public:
            push( ascii& p_stream, const event_data& p_data )
            {
                p_stream <<
                p_data.size() << " " <<
                p_data.frequency_interval() << " " <<
                p_data.frequency_index() << " " <<
                p_data.time_interval() << " " <<
                p_data.time_index() << "\n" <<
                p_data.events() << "\n";
            }
    };

    template< >
    class binary::pull< event_data >
    {
        public:
            pull( binary& p_stream, event_data& p_data )
            {
                p_stream >>
                p_data.size() >>
                p_data.frequency_interval() >>
                p_data.frequency_index() >>
                p_data.time_interval() >>
                p_data.time_index() >>
                p_data.events();
            }
    };
    template< >
    class binary::push< event_data >
    {
        public:
            push( binary& p_stream, const event_data& p_data )
            {
                p_stream <<
                p_data.size() <<
                p_data.frequency_interval() <<
                p_data.frequency_index() <<
                p_data.time_interval() <<
                p_data.time_index() <<
                p_data.events();
            }
    };

}

#endif
