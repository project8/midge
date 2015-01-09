#ifndef _midge_line_data_hh_
#define _midge_line_data_hh_

#include "line.hh"

#include <vector>
using std::vector;

namespace midge
{

    class line_data
    {
        public:
            line_data();
            virtual ~line_data();

        public:
            ;referrable( vector< line >, lines )
            ;referrable( count_t, size )
            ;referrable( real_t, time_interval )
            ;referrable( count_t, time_index )
            ;referrable( real_t, frequency_interval )
            ;referrable( count_t, frequency_index )
    };

    template< >
    class ascii::pull< line_data >
    {
        public:
            pull( ascii& p_stream, line_data& p_data )
            {
                p_stream >>
                p_data.size() >>
                p_data.frequency_interval() >>
                p_data.frequency_index() >>
                p_data.time_interval() >>
                p_data.time_index() >>
                p_data.lines();
            }
    };
    template< >
    class ascii::push< line_data >
    {
        public:
            push( ascii& p_stream, const line_data& p_data )
            {
                p_stream <<
                p_data.size() << " " <<
                p_data.frequency_interval() << " " <<
                p_data.frequency_index() << " " <<
                p_data.time_interval() << " " <<
                p_data.time_index() << "\n" <<
                p_data.lines() << "\n";
            }
    };

    template< >
    class binary::pull< line_data >
    {
        public:
            pull( binary& p_stream, line_data& p_data )
            {
                p_stream >>
                p_data.size() >>
                p_data.frequency_interval() >>
                p_data.frequency_index() >>
                p_data.time_interval() >>
                p_data.time_index() >>
                p_data.lines();
            }
    };
    template< >
    class binary::push< line_data >
    {
        public:
            push( binary& p_stream, const line_data& p_data )
            {
                p_stream <<
                p_data.size() <<
                p_data.frequency_interval() <<
                p_data.frequency_index() <<
                p_data.time_interval() <<
                p_data.time_index() <<
                p_data.lines();
            }
    };

}

#endif
