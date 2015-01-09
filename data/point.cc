#include "point.hh"

#include <cmath>

namespace midge
{

    count_t point::s_id = 0;
    real_t point::s_threshold = 0.;
    real_t point::s_low_factor = 0.;
    real_t point::s_low_power = 0.;
    real_t point::s_high_factor = 0.;
    real_t point::s_high_power = 0.;
    rf_data* point::s_background_data = NULL;
    rtf_data* point::s_signal_data = NULL;

    point::point() :
                f_free( true ),
                f_id( 0 ),
                f_time( 0. ),
                f_frequency( 0. ),
                f_ratio( 0. ),
                f_score( 0. )
    {
    }
    point::point( const point& p_copy ) :
                f_free( p_copy.f_free ),
                f_id( p_copy.f_id ),
                f_time( p_copy.f_time ),
                f_frequency( p_copy.f_frequency ),
                f_ratio( p_copy.f_ratio ),
                f_score( p_copy.f_score )
    {
    }
    point::~point()
    {
    }

    void point::initialize( const count_t& p_index )
    {
        f_free = true;
        f_id = ++s_id;

        f_time = s_signal_data->get_time_interval() * s_signal_data->get_time_index();
        f_frequency = s_signal_data->get_frequency_interval() * (s_signal_data->get_frequency_index() + p_index);

        f_ratio = s_signal_data->at( p_index ) / s_background_data->at( p_index );
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
