#include "point_data.hh"

namespace midge
{

    point_data::point_data() :
            f_points(),
            f_time_interval( 1. ),
            f_time_index( 0 ),
            f_frequency_interval( 1. ),
            f_frequency_index( 0 )
    {
    }
    point_data::~point_data()
    {
    }

}