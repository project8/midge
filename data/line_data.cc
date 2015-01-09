#include "line_data.hh"

namespace midge
{

    line_data::line_data() :
                f_lines(),
                f_size( 0 ),
                f_time_interval( 1. ),
                f_time_index( 0 ),
                f_frequency_interval( 1. ),
                f_frequency_index( 0 )
    {
    }
    line_data::~line_data()
    {
    }

}
