#ifndef midge_thread_hh_
#define midge_thread_hh_

#include "typenull.hh"

#include "shared_cancel.hh"

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

            // member function; shared cancel
            template< class x_type, class x_r, class... Arguments >
            void bind_start( x_type* p_object, x_r(x_type::*p_member)( shared_cancel_t, Arguments... ), Arguments... params );

            // member function; without shared cancel
            template< class x_type, class x_r, class... Arguments >
            void bind_start( x_type* p_object, x_r(x_type::*p_member)( Arguments... ), Arguments... params );

            // non-member function; shared cancel
            template< class x_r, class... Arguments >
            void bind_start( x_r (*p_member)( shared_cancel_t, Arguments... ), Arguments... params );

            template< class x_type, class x_r, class... Arguments >
            void bind_stop( x_type* p_object, x_r(x_type::*p_member)( Arguments... ), Arguments... params );

            template< class x_r, class... Arguments >
            void bind_stop( x_r (*p_member)( Arguments... ), Arguments... params );

            void start();

            void join();

            void stop();

            state get_state();

        private:
            class callable
            {
                public:
                    callable();
                    virtual ~callable();

                    virtual void execute() {};
            };

            template< class x_type, class x_r, class... Arguments >
            class _callable;

            template< class x_type, class x_r, class... Arguments >
            class _callable_cancelable;

            void thread_start();

            std::thread f_thread;
            std::mutex f_thread_mutex;
            std::atomic< state > f_state;
            shared_cancel_t f_canceled;
            std::shared_ptr< callable > f_start;
            std::shared_ptr< callable > f_stop;
    };

    inline thread::callable::callable()
    {
    }
    inline thread::callable::~callable()
    {
    }

    template< class x_type, class x_r, class... Arguments >
    class thread::_callable :
        public thread::callable
    {
        public:
            _callable( x_type* p_object, x_r (x_type::*p_member)( shared_cancel_t, Arguments... ), shared_cancel_t a_canceled, Arguments... params ) :
                        f_bound( std::bind( p_member, p_object, a_canceled, params... ) )
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
            std::function< x_r () > f_bound;
    };
    template< class x_type, class x_r, class... Arguments >
    void thread::bind_start( x_type* p_object, x_r(x_type::*p_member)( shared_cancel_t, Arguments... ), Arguments... params )
    {
        f_start.reset( new _callable< x_type, x_r, Arguments... >( p_object, p_member, f_canceled, params... ) );
    }
    template< class x_type, class x_r, class... Arguments >
    void thread::bind_stop( x_type* p_object, x_r(x_type::*p_member)( Arguments... ), Arguments... params )
    {
        f_stop.reset( new _callable< x_type, x_r, Arguments... >( p_object, p_member, params... ) );
    }

    template< class x_type, class x_r, class... Arguments >
    class thread::_callable_cancelable :
        public thread::callable
    {
        public:
            _callable_cancelable( x_type* p_object, x_r (x_type::*p_member)( Arguments... ), Arguments... params ) :
                        f_bound( std::bind( p_member, p_object, params... ) )
            {
            }
            virtual ~_callable_cancelable() noexcept
            {
            }

            void execute()
            {
                f_bound();
                return;
            }

        private:
            std::function< x_r () > f_bound;
    };
    template< class x_type, class x_r, class... Arguments >
    void thread::bind_start( x_type* p_object, x_r(x_type::*p_member)( Arguments... ), Arguments... params )
    {
        f_start.reset( new _callable_cancelable< x_type, x_r, Arguments... >( p_object, p_member, params... ) );
    }

    template< class x_r, class... Arguments >
    class thread::_callable< _, x_r, Arguments... > :
        public thread::callable
    {
        public:
            _callable( x_r (*p_function)( shared_cancel_t, Arguments... ), shared_cancel_t a_canceled, Arguments... params ) :
                        f_bound( std::bind( p_function, a_canceled, params... ) )
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
    void thread::bind_start( x_r (*p_function)( shared_cancel_t, Arguments... ), Arguments... params )
    {
        f_start.reset( new _callable< _, x_r, Arguments... >( p_function, f_canceled, params... ) );
    }
    template< class x_r, class... Arguments >
    void thread::bind_stop( x_r (*p_function)( Arguments... ), Arguments... params )
    {
        f_stop.reset( new _callable< _, x_r, Arguments... >( p_function, params... ) );
    }

    inline thread::state thread::get_state()
    {
        return f_state.load();
    }


}

#endif
