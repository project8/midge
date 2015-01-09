#include "event_data.hh"

namespace midge
{

    event_data::event_data() :
                f_events(),
                f_size( 0 ),
                f_frequency_interval( 1. ),
                f_frequency_index( 0 ),
                f_time_interval( 1. ),
                f_time_index( 0 )
    {
    }
    event_data::~event_data()
    {
    }

}
