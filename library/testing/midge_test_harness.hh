#ifndef _midge_test_harness_hh_
#define _midge_test_harness_hh_

#include "diptera.hh"

#include <chrono>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <string>
#include <thread>

namespace midge
{
    namespace testing
    {
        /*!
         @struct run_result
         @brief Outcome of a watchdogged diptera run.
        */
        struct run_result
        {
            std::exception_ptr f_exception;  ///< non-null if a node called throw_ex()
            bool f_timed_out;               ///< true if the watchdog had to intervene

            run_result() : f_exception(), f_timed_out( false ) {}

            /// Clean completion: no exception, no timeout
            bool ok() const { return ! f_exception && ! f_timed_out; }
        };

        /*!
         @brief Run a diptera pipeline with a watchdog that cancels it after a timeout.

         @details
         Many midge nodes loop on `while( ! is_canceled() )` with no break on s_stop or
         s_exit, so a pipeline whose consumer does not cancel will block forever in
         diptera::run()'s thread join. Without a watchdog, a regression hangs CI instead
         of failing it -- so the timeout is a hard requirement, not a convenience.

         Cancelling from the watchdog thread is safe: diptera::run() releases
         f_threads_mutex before joining, so do_cancellation() can acquire it.

         Note that diptera::run() itself sleeps 100 ms for thread spin-up and
         do_cancellation() sleeps 500 ms between node tiers, so a "fast" run still takes
         roughly 600+ ms. Pick a timeout well above that -- the default is 10 s.

         @param a_midge   the diptera instance, with nodes already added and joined
         @param a_nodes   colon-separated node names to run, e.g. "prod:node:cons"
         @param a_timeout_ms  wall-clock budget before the watchdog cancels
        */
        inline run_result run_with_watchdog( diptera& a_midge,
                                             const std::string& a_nodes,
                                             unsigned a_timeout_ms = 10000 )
        {
            run_result t_result;

            std::mutex t_mutex;
            std::condition_variable t_cv;
            bool t_finished = false;

            std::thread t_watchdog( [&]()
            {
                std::unique_lock< std::mutex > t_lock( t_mutex );
                if( ! t_cv.wait_for( t_lock,
                                     std::chrono::milliseconds( a_timeout_ms ),
                                     [&](){ return t_finished; } ) )
                {
                    t_result.f_timed_out = true;
                    t_lock.unlock();
                    a_midge.cancel();
                }
            } );

            t_result.f_exception = a_midge.run( a_nodes );

            {
                std::unique_lock< std::mutex > t_lock( t_mutex );
                t_finished = true;
            }
            t_cv.notify_all();
            t_watchdog.join();

            return t_result;
        }
    }
}

#endif
