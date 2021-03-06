/*
 * stream_timer.cc
 *
 *  Created on: Feb 26, 2016
 *      Author: nsoblath
 */

#include "stream_timer.hh"

#ifdef ENABLE_STREAM_TIMING

#include "node.hh"

#include "logger.hh"

#include <sstream>

namespace midge
{

    LOGGER( mlog, "stream_timer" );

    stream_timer::stream_timer() :
            f_state( state::starting ),
            f_total_work_time( ns_t::zero() ),
            f_n_work_periods( 0 ),
            f_total_blocked_time( ns_t::zero() ),
            f_n_blocked_periods( 0 ),
            f_timer_start( time_point_t::min() ),
            f_unpaused_buffer( ns_t::zero() ),
            f_unpaused_state( state::paused )
    {
    }

    stream_timer::~stream_timer()
    {
    }

    void stream_timer::report( const std::string& a_label ) const
    {
        typedef std::chrono::duration< double, std::ratio<1,1> > s_flt_t;

        s_flt_t t_total_work_time_s = std::chrono::duration_cast< s_flt_t >( f_total_work_time );
        s_flt_t t_total_blocked_time_s = std::chrono::duration_cast< s_flt_t >( f_total_blocked_time );

        s_flt_t t_work_period = s_flt_t::zero();
        double t_working_rate = std::numeric_limits< double >::infinity();
        if( f_n_work_periods != 0 )
        {
            t_work_period = t_total_work_time_s / f_n_work_periods;
            t_working_rate = 1. / t_work_period.count();
        }

        s_flt_t t_blocked_period = s_flt_t::zero();
        if( f_n_blocked_periods != 0 ) t_blocked_period = t_total_blocked_time_s / f_n_blocked_periods;

        std::stringstream t_timer_report;
        t_timer_report << "Stream timer report for " << a_label << "\n";
        t_timer_report << "-----------------------------------------------------------------------------------------------\n";
        if( f_state != state::starting )
        {
            t_timer_report << " * Stream advanced " << f_n_blocked_periods << " times\n";
            t_timer_report << " * Total time spent working: " << t_total_work_time_s.count() << " s\n";
            t_timer_report << " * Total time waiting to advance: " << t_total_blocked_time_s.count() << " s\n";
            t_timer_report << " * Mean working period: " << t_work_period.count() << " s\n";
            t_timer_report << " * Mean blocked period: " << t_blocked_period.count() << " s\n";
            t_timer_report << " * Stream working rate: " << t_working_rate << " s^-1\n";
        }
        else
        {
            t_timer_report << "Timer was unused\n";
        }
        LINFO( mlog, t_timer_report.str() );
    }

    void stream_timer::reset()
    {
        f_state = state::paused;
        f_total_work_time = ns_t::zero();
        f_n_work_periods = 0;
        f_total_blocked_time = ns_t::zero();
        f_n_blocked_periods = 0;
        f_timer_start = time_point_t::min();
        f_unpaused_buffer = ns_t::zero();
        f_unpaused_state = state::paused;
        return;
    }


} /* namespace midge */

#endif /* ENABLE_STREAM_TIMER */
