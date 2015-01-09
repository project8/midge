#include "event.hh"

#include <limits>
using std::numeric_limits;

#include <cmath>

#include "coremsg.hh"

namespace midge
{
    count_t event::s_id = 0;
    real_t event::s_frequency = 0.;
    real_t event::s_time = 0.;
    real_t event::s_tolerance = 0.;
    real_t event::s_delay = 0.;
    line_data* event::s_line_data = NULL;

    event::event() :
                f_id( 0 ),
                f_time( 0. ),
                f_frequency( 0. ),
                f_slope( 0. ),
                f_lines()
    {
    }
    event::event( const event& p_copy ) :
                f_id( p_copy.f_id ),
                f_time( p_copy.f_time ),
                f_frequency( p_copy.f_frequency ),
                f_slope( p_copy.f_slope ),
                f_lines( p_copy.f_lines )
    {
    }
    event::~event()
    {
    }

    void event::initialize( const count_t& p_index )
    {
        f_id = ++s_id;

        line* t_line = &(s_line_data->lines().at( p_index ));
        f_time = t_line->time();
        f_frequency = t_line->frequency();
        f_slope = t_line->slope();
        f_lines.push_back( *t_line );

        return;
    }

    bool event::update()
    {
        count_t t_index;

        line* t_line;
        real_t t_line_time;
        real_t t_line_frequency;
        real_t t_line_error;

        line* t_min = NULL;
        real_t t_min_error = numeric_limits< real_t >::max();
        real_t t_min_time = numeric_limits< real_t >::max();

        for( t_index = 0; t_index < s_line_data->lines().size(); t_index++ )
        {
            t_line = &(s_line_data->lines().at( t_index ));

            if( t_line->free() == false )
            {
                continue;
            }

            t_line_time = t_line->time() - (f_lines.back().time() + f_lines.back().duration());
            t_line_frequency = t_line->frequency() - (f_lines.back().frequency() + f_lines.back().slope() * f_lines.back().duration());
            t_line_error = sqrt( (t_line_time * t_line_time)/(s_time * s_time) + (t_line_frequency * t_line_frequency)/(s_frequency * s_frequency) );

            if( t_min_time > t_line_time )
            {
                t_min_time = t_line_time;
            }

            if( t_min_error > t_line_error )
            {
                t_min_error = t_line_error;
                t_min = t_line;
            }
        }

        if( t_min == NULL )
        {
            return true;
        }

        if( t_min_time > s_delay )
        {
            return false;
        }

        if( t_min_error > s_tolerance )
        {
            return true;
        }

        f_lines.push_back( *t_min );
        t_min->free() = false;

        return true;
    }

}
