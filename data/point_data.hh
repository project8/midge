#ifndef _midge_point_data_hh_
#define _midge_point_data_hh_

#include "point.hh"

#include <vector>
using std::vector;

namespace midge
{

    class point_data
    {
        public:
            point_data();
            virtual ~point_data();

        public:
            ;referrable( vector< point >, points )
            ;referrable( count_t, size )
            ;referrable( real_t, frequency_interval )
            ;referrable( count_t, frequency_index )
            ;referrable( real_t, time_interval )
            ;referrable( count_t, time_index )
    };

    template< >
    class ascii::pull< point_data >
    {
        public:
            pull( ascii& p_stream, point_data& p_data )
            {
                p_stream >>
                p_data.points() >>
                p_data.size() >>
                p_data.frequency_interval() >>
                p_data.frequency_index() >>
                p_data.time_interval() >>
                p_data.time_index();
            }
    };
    template< >
    class ascii::push< point_data >
    {
        public:
            push( ascii& p_stream, const point_data& p_data )
            {
                p_stream <<
                p_data.points() << '\n' <<
                p_data.size() << " " <<
                p_data.frequency_interval() << " " <<
                p_data.frequency_index() << " " <<
                p_data.time_interval() << " " <<
                p_data.time_index() << '\n';
            }
    };

    template< >
    class binary::pull< point_data >
    {
        public:
            pull( binary& p_stream, point_data& p_data )
            {
                p_stream >>
                p_data.points() >>
                p_data.size() >>
                p_data.frequency_interval() >>
                p_data.frequency_index() >>
                p_data.time_interval() >>
                p_data.time_index();
            }
    };
    template< >
    class binary::push< point_data >
    {
        public:
            push( binary& p_stream, const point_data& p_data )
            {
                p_stream <<
                p_data.points() <<
                p_data.size() <<
                p_data.frequency_interval() <<
                p_data.frequency_index() <<
                p_data.time_interval() <<
                p_data.time_index();
            }
    };

}

#endif
