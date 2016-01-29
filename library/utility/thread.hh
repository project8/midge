#ifndef midge_thread_hh_
#define midge_thread_hh_

#include "typenull.hh"

#include <atomic>
#include <functional>
#include <thread>
#include <mutex>

namespace midge
{

    class thread
    {
        public:
            typedef enum
            {
                e_ready, e_executing, e_executed, e_cancelling, e_cancelled
            } state;

        public:
            thread();
            virtual ~thread();

            void start();

            template< class x_type, class x_r, class... Arguments >
            void start( x_type* p_object, x_r(x_type::*p_member)( Arguments... ), Arguments... params );

            template< class x_r, class... Arguments >
            void start( x_r (*p_member)( Arguments... ), Arguments... params );

            void join();

            void stop();

            template< class x_type, class x_r, class... Arguments >
            void stop( x_type* p_object, x_r(x_type::*p_member)( Arguments... ), Arguments... params );

            template< class x_r, class... Arguments >
            void stop( x_r (*p_member)( Arguments... ), Arguments... params );

            state get_state();

        private:
            class callable
            {
                public:
                    callable();
                    virtual ~callable();

                    virtual void execute() = 0;
            };

            template< class x_type, class x_r, class... Arguments > //class x_a1 = _, class x_a2 = _, class x_a3 = _ >
            class _callable;

            void thread_start();
            void thread_stop();

            std::thread f_thread;
            std::mutex f_thread_mutex;
            std::atomic< state > f_state;
            std::atomic< bool > f_canceled;
            callable* f_start;
            callable* f_stop;
    };

    inline thread::callable::callable()
    {
    }
    inline thread::callable::~callable()
    {
    }

    template< class x_type, class x_r, class... Arguments > //class x_a1, class x_a2, class x_a3 >
    class thread::_callable :
        public thread::callable
    {
        public:
            _callable( x_type* p_object, x_r (x_type::*p_member)( Arguments... ), Arguments... params ) :
                        f_bound( std::bind( p_member, p_object, params... ) )
            {
            }
            virtual ~_callable() noexcept
            {
            }

            void execute()
            {
                f_bound();
                return;
            }

        private:
            std::function< x_r ( Arguments... ) > f_bound;
    };
    template< class x_type, class x_r, class... Arguments >
    inline void thread::start( x_type* p_object, x_r(x_type::*p_member)( Arguments... ), Arguments... params )
    {
        f_start = new _callable< x_type, x_r, Arguments... >( p_object, p_member, params... );
    }
    template< class x_type, class x_r, class... Arguments >
    inline void thread::stop( x_type* p_object, x_r(x_type::*p_member)( Arguments... ), Arguments... params )
    {
        f_stop = new _callable< x_type, x_r, Arguments... >( p_object, p_member, params... );
    }

    template< class x_r, class... Arguments >
    class thread::_callable< _, x_r, Arguments... > :
        public thread::callable
    {
        public:
            _callable( x_r (*p_function)( Arguments... ), Arguments... params ) :
                        f_bound( std::bind( p_function, params... ) )
            {
            }
            virtual ~_callable()
            {
            }

            void execute()
            {
                f_bound();
                return;
            }

        private:
            std::function< x_r > f_bound;
    };
    template< class x_r, class... Arguments >
    inline void thread::start( x_r (*p_function)( Arguments... ), Arguments... params )
    {
        f_start = new _callable< _, x_r, Arguments... >( p_function, params... );
    }
    template< class x_r, class... Arguments >
    inline void thread::stop( x_r (*p_function)( Arguments... ), Arguments... params )
    {
        f_stop = new _callable< _, x_r, Arguments... >( p_function, params... );
    }

}

#endif
