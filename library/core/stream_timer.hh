/*
 * stream_timer.hh
 *
 *  Created on: Feb 26, 2016
 *      Author: nsoblath
 */

#ifndef MIDGE_STREAM_TIMER_HH_
#define MIDGE_STREAM_TIMER_HH_

#ifdef ENABLE_STREAM_TIMING
#include <chrono>
#include <string>

namespace midge
{
    class node;

    class stream_timer
    {
        private:
            typedef std::chrono::steady_clock steady_clock_t;
            typedef std::chrono::time_point< steady_clock_t > time_point_t;
            typedef std::chrono::nanoseconds ns_t;

        public:
            stream_timer();
            virtual ~stream_timer();

            void increment_begin();
            void increment_locked();

            void pause_timer();
            void resume_timer();

            void report( const std::string& a_label ) const;

            void reset();

        private:
            enum class state
            {
                starting,
                working,
                blocked,
                paused
            };
            state f_state;

            ns_t f_total_work_time;
            uint64_t f_n_work_periods;

            ns_t f_total_blocked_time;
            uint64_t f_n_blocked_periods;

            time_point_t f_timer_start;
            ns_t f_unpaused_buffer;
            state f_unpaused_state;
    };

    inline void stream_timer::increment_begin()
    {
        if( f_state == state::blocked || f_unpaused_state == state::blocked ) return;
        f_total_work_time += f_unpaused_buffer;
        if( f_state == state::working ) f_total_work_time += steady_clock_t::now() - f_timer_start;
        if( f_state != state::starting ) ++f_n_work_periods;
        f_unpaused_buffer = ns_t::zero();
        f_state = state::blocked;
        f_timer_start = steady_clock_t::now();
        return;
    }

    inline void stream_timer::increment_locked()
    {
        if( f_state == state::working || f_unpaused_state == state::working ) return;
        f_total_blocked_time += f_unpaused_buffer;
        if( f_state == state::blocked ) f_total_blocked_time += steady_clock_t::now() - f_timer_start;
        if( f_state != state::starting ) ++f_n_blocked_periods;
        f_unpaused_buffer = ns_t::zero();
        f_state = state::working;
        f_timer_start = steady_clock_t::now();
        return;
    }

    inline void stream_timer::pause_timer()
    {
        if( f_state == state::paused || f_state == state::starting ) return;
        f_unpaused_buffer += steady_clock_t::now() - f_timer_start;
        f_unpaused_state = f_state;
        f_state = state::paused;
        return;
    }

    inline void stream_timer::resume_timer()
    {
        if( f_state != state::paused ) return;
        f_state = f_unpaused_state;
        f_unpaused_state = state::paused;
        f_timer_start = std::chrono::steady_clock::now();
        return;
    }

} /* namespace midge */

#endif /* ENABLE_STREAM_TIMING */

#ifdef ENABLE_STREAM_TIMING
#define IF_STREAM_TIMING_ENABLED( x_line ) x_line
#else
#define IF_STREAM_TIMING_ENABLED( x_line )
#endif

#endif /* MIDGE_STREAM_TIMER_HH_ */
