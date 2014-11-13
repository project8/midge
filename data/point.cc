#include "point.hh"

#include <cmath>

namespace midge
{

    real_t point::s_threshold = 0.;
    real_t point::s_low_factor = 0.;
    real_t point::s_low_power = 0.;
    real_t point::s_high_factor = 0.;
    real_t point::s_high_power = 0.;

    point::point() :
            f_id( 0 ),
            f_time( -1. ),
            f_frequency( -1. ),
            f_ratio( -1. ),
            f_score( 0. )
    {
    }
    point::point( const point& a_copy ) :
            f_id( a_copy.f_id ),
            f_time( a_copy.f_time ),
            f_frequency( a_copy.f_frequency ),
            f_ratio( a_copy.f_ratio ),
            f_score( a_copy.f_score )
    {
    }
    point::~point()
    {
    }

    void point::update()
    {
        if( f_ratio > s_threshold )
        {
            f_score = s_high_factor * pow( f_ratio - s_threshold + 1., s_high_power );
        }
        else
        {
            f_score = -s_low_factor * pow( s_threshold - f_ratio + 1., s_low_power );
        }

        return;
    }

}
