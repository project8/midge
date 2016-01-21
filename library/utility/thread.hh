#ifndef midge_thread_hh_
#define midge_thread_hh_

#include "typenull.hh"

#include <atomic>
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

            template< class x_type, class x_r >
            void start( x_type* p_object, x_r (x_type::*p_member)() );

            template< class x_type, class x_r, class x_a1 >
            void start( x_type* p_object, x_r (x_type::*p_member)( x_a1 ), x_a1 p_a1 );

            template< class x_type, class x_r, class x_a1, class x_a2 >
            void start( x_type* p_object, x_r (x_type::*p_member)( x_a1, x_a2 ), x_a1 p_a1, x_a2 p_a2 );

            template< class x_type, class x_r, class x_a1, class x_a2, class x_a3 >
            void start( x_type* p_object, x_r (x_type::*p_member)( x_a1, x_a2, x_a3 ), x_a1 p_a1, x_a2 p_a2, x_a3 p_a3 );

            template< class x_r >
            void start( x_r (*p_member)() );

            template< class x_r, class x_a1 >
            void start( x_r (*p_member)( x_a1 ), x_a1 p_a1 );

            template< class x_r, class x_a1, class x_a2 >
            void start( x_r (*p_member)( x_a1, x_a2 ), x_a1 p_a1, x_a2 p_a2 );

            template< class x_r, class x_a1, class x_a2, class x_a3 >
            void start( x_r (*p_member)( x_a1, x_a2, x_a3 ), x_a1 p_a1, x_a2 p_a2, x_a3 p_a3 );

            void join();

            void stop();

            template< class x_type, class x_r >
            void stop( x_type* p_object, x_r (x_type::*p_member)() );

            template< class x_type, class x_r, class x_a1 >
            void stop( x_type* p_object, x_r (x_type::*p_member)( x_a1 ), x_a1 p_a1 );

            template< class x_type, class x_r, class x_a1, class x_a2 >
            void stop( x_type* p_object, x_r (x_type::*p_member)( x_a1, x_a2 ), x_a1 p_a1, x_a2 p_a2 );

            template< class x_type, class x_r, class x_a1, class x_a2, class x_a3 >
            void stop( x_type* p_object, x_r (x_type::*p_member)( x_a1, x_a2, x_a3 ), x_a1 p_a1, x_a2 p_a2, x_a3 p_a3 );

            template< class x_r >
            void stop( x_r (*p_member)() );

            template< class x_r, class x_a1 >
            void stop( x_r (*p_member)( x_a1 ), x_a1 p_a1 );

            template< class x_r, class x_a1, class x_a2 >
            void stop( x_r (*p_member)( x_a1, x_a2 ), x_a1 p_a1, x_a2 p_a2 );

            template< class x_r, class x_a1, class x_a2, class x_a3 >
            void stop( x_r (*p_member)( x_a1, x_a2, x_a3 ), x_a1 p_a1, x_a2 p_a2, x_a3 p_a3 );

            state get_state();

        private:
            class callable
            {
                public:
                    callable();
                    virtual ~callable();

                    virtual void execute() = 0;
            };

            template< class x_type, class x_r, class x_a1 = _, class x_a2 = _, class x_a3 = _ >
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

    template< class x_type, class x_r, class x_a1, class x_a2, class x_a3 >
    class thread::_callable :
        public thread::callable
    {
        public:
            _callable( x_type* p_object, x_r (x_type::*p_member)( x_a1, x_a2, x_a3 ), x_a1 p_a1, x_a2 p_a2, x_a3 p_a3 ) :
                        f_object( p_object ),
                        f_member( p_member ),
                        f_a1( p_a1 ),
                        f_a2( p_a2 ),
                        f_a3( p_a3 )
            {
            }
            virtual ~_callable()
            {
            }

            void execute()
            {
                (f_object->*f_member)( f_a1, f_a2, f_a3 );
                return;
            }

        private:
            x_type* f_object;
            x_r (x_type::*f_member)( x_a1, x_a2, x_a3 );
            x_a1 f_a1;
            x_a2 f_a2;
            x_a3 f_a3;
    };
    template< class x_type, class x_r, class x_a1, class x_a2, class x_a3 >
    inline void thread::start( x_type* p_object, x_r (x_type::*p_member)( x_a1, x_a2, x_a3 ), x_a1 p_a1, x_a2 p_a2, x_a3 p_a3 )
    {
        f_start = new _callable< x_type, x_r, x_a1, x_a2, x_a3 >( p_object, p_member, p_a1, p_a2, p_a3 );
    }
    template< class x_type, class x_r, class x_a1, class x_a2, class x_a3 >
    inline void thread::stop( x_type* p_object, x_r (x_type::*p_member)( x_a1, x_a2, x_a3 ), x_a1 p_a1, x_a2 p_a2, x_a3 p_a3 )
    {
        f_stop = new _callable< x_type, x_r, x_a1, x_a2, x_a3 >( p_object, p_member, p_a1, p_a2, p_a3 );
    }

    template< class x_type, class x_r, class x_a1, class x_a2 >
    class thread::_callable< x_type, x_r, x_a1, x_a2, _ > :
        public thread::callable
    {
        public:
            _callable( x_type* p_object, x_r (x_type::*p_member)( x_a1, x_a2 ), x_a1 p_a1, x_a2 p_a2 ) :
                        f_object( p_object ),
                        f_member( p_member ),
                        f_a1( p_a1 ),
                        f_a2( p_a2 )
            {
            }
            virtual ~_callable()
            {
            }

            void execute()
            {
                (f_object->*f_member)( f_a1, f_a2 );
                return;
            }

        private:
            x_type* f_object;
            x_r (x_type::*f_member)( x_a1, x_a2 );
            x_a1 f_a1;
            x_a2 f_a2;
    };
    template< class x_type, class x_r, class x_a1, class x_a2 >
    inline void thread::start( x_type* p_object, x_r (x_type::*p_member)( x_a1, x_a2 ), x_a1 p_a1, x_a2 p_a2 )
    {
        f_start = new _callable< x_type, x_r, x_a1, x_a2 >( p_object, p_member, p_a1, p_a2 );
    }
    template< class x_type, class x_r, class x_a1, class x_a2 >
    inline void thread::stop( x_type* p_object, x_r (x_type::*p_member)( x_a1, x_a2 ), x_a1 p_a1, x_a2 p_a2 )
    {
        f_stop = new _callable< x_type, x_r, x_a1, x_a2 >( p_object, p_member, p_a1, p_a2 );
    }

    template< class x_type, class x_r, class x_a1 >
    class thread::_callable< x_type, x_r, x_a1, _, _ > :
        public thread::callable
    {
        public:
            _callable( x_type* p_object, x_r (x_type::*p_member)( x_a1 ), x_a1 p_a1 ) :
                        f_object( p_object ),
                        f_member( p_member ),
                        f_a1( p_a1 )
            {
            }
            virtual ~_callable()
            {
            }

            void execute()
            {
                (f_object->*f_member)( f_a1 );
                return;
            }

        private:
            x_type* f_object;
            x_r (x_type::*f_member)( x_a1 );
            x_a1 f_a1;
    };
    template< class x_type, class x_r, class x_a1 >
    inline void thread::start( x_type* p_object, x_r (x_type::*p_member)( x_a1 ), x_a1 p_a1 )
    {
        f_start = new _callable< x_type, x_r, x_a1 >( p_object, p_member, p_a1 );
    }
    template< class x_type, class x_r, class x_a1 >
    inline void thread::stop( x_type* p_object, x_r (x_type::*p_member)( x_a1 ), x_a1 p_a1 )
    {
        f_stop = new _callable< x_type, x_r, x_a1 >( p_object, p_member, p_a1 );
    }

    template< class x_type, class x_r >
    class thread::_callable< x_type, x_r, _, _, _ > :
        public thread::callable
    {
        public:
            _callable( x_type* p_object, x_r (x_type::*p_member)() ) :
                        f_object( p_object ),
                        f_member( p_member )
            {
            }
            virtual ~_callable()
            {
            }

            void execute()
            {
                (f_object->*f_member)();
                return;
            }

        private:
            x_type* f_object;
            x_r (x_type::*f_member)();
    };
    template< class x_type, class x_r >
    inline void thread::start( x_type* p_object, x_r (x_type::*p_member)() )
    {
        f_start = new _callable< x_type, x_r >( p_object, p_member );
    }
    template< class x_type, class x_r >
    inline void thread::stop( x_type* p_object, x_r (x_type::*p_member)() )
    {
        f_stop = new _callable< x_type, x_r >( p_object, p_member );
    }

    template< class x_r, class x_a1, class x_a2, class x_a3 >
    class thread::_callable< _, x_r, x_a1, x_a2, x_a3 > :
        public thread::callable
    {
        public:
            _callable( x_r (*p_function)( x_a1, x_a2, x_a3 ), x_a1 p_a1, x_a2 p_a2, x_a3 p_a3 ) :
                        f_function( p_function ),
                        f_a1( p_a1 ),
                        f_a2( p_a2 ),
                        f_a3( p_a3 )
            {
            }
            virtual ~_callable()
            {
            }

            void execute()
            {
                (*f_function)( f_a1, f_a2, f_a3 );
                return;
            }

        private:
            x_r (*f_function)( x_a1, x_a2, x_a3 );
            x_a1 f_a1;
            x_a2 f_a2;
            x_a3 f_a3;
    };
    template< class x_r, class x_a1, class x_a2, class x_a3 >
    inline void thread::start( x_r (*p_function)( x_a1, x_a2, x_a3 ), x_a1 p_a1, x_a2 p_a2, x_a3 p_a3 )
    {
        f_start = new _callable< _, x_r, x_a1, x_a2, x_a3 >( p_function, p_a1, p_a2, p_a3 );
    }
    template< class x_r, class x_a1, class x_a2, class x_a3 >
    inline void thread::stop( x_r (*p_function)( x_a1, x_a2, x_a3 ), x_a1 p_a1, x_a2 p_a2, x_a3 p_a3 )
    {
        f_stop = new _callable< _, x_r, x_a1, x_a2, x_a3 >( p_function, p_a1, p_a2, p_a3 );
    }

    template< class x_r, class x_a1, class x_a2 >
    class thread::_callable< _, x_r, x_a1, x_a2, _ > :
        public thread::callable
    {
        public:
            _callable( x_r (*p_function)( x_a1, x_a2 ), x_a1 p_a1, x_a2 p_a2 ) :
                        f_function( p_function ),
                        f_a1( p_a1 ),
                        f_a2( p_a2 )
            {
            }
            virtual ~_callable()
            {
            }

            void execute()
            {
                (*f_function)( f_a1, f_a2 );
                return;
            }

        private:
            x_r (*f_function)( x_a1, x_a2 );
            x_a1 f_a1;
            x_a2 f_a2;
    };
    template< class x_r, class x_a1, class x_a2 >
    inline void thread::start( x_r (*p_function)( x_a1, x_a2 ), x_a1 p_a1, x_a2 p_a2 )
    {
        f_start = new _callable< _, x_r, x_a1, x_a2 >( p_function, p_a1, p_a2 );
    }
    template< class x_r, class x_a1, class x_a2 >
    inline void thread::stop( x_r (*p_function)( x_a1, x_a2 ), x_a1 p_a1, x_a2 p_a2 )
    {
        f_stop = new _callable< _, x_r, x_a1, x_a2 >( p_function, p_a1, p_a2 );
    }

    template< class x_r, class x_a1 >
    class thread::_callable< _, x_r, x_a1, _, _ > :
        public thread::callable
    {
        public:
            _callable( x_r (*p_function)( x_a1 ), x_a1 p_a1 ) :
                        f_function( p_function ),
                        f_a1( p_a1 )
            {
            }
            virtual ~_callable()
            {
            }

            void execute()
            {
                (*f_function)( f_a1 );
                return;
            }

        private:
            x_r (*f_function)( x_a1 );
            x_a1 f_a1;
    };
    template< class x_r, class x_a1 >
    inline void thread::start( x_r (*p_function)( x_a1 ), x_a1 p_a1 )
    {
        f_start = new _callable< _, x_r, x_a1 >( p_function, p_a1 );
    }
    template< class x_r, class x_a1 >
    inline void thread::stop( x_r (*p_function)( x_a1 ), x_a1 p_a1 )
    {
        f_stop = new _callable< _, x_r, x_a1 >( p_function, p_a1 );
    }

    template< class x_r >
    class thread::_callable< _, x_r, _, _, _ > :
        public thread::callable
    {
        public:
            _callable( x_r (*p_function)() ) :
                        f_function( p_function )
            {
            }
            virtual ~_callable()
            {
            }

            void execute()
            {
                (*f_function)();
                return;
            }

        private:
            x_r (*f_function)();
    };
    template< class x_r >
    inline void thread::start( x_r (*p_function)() )
    {
        f_start = new _callable< _, x_r >( p_function );
    }
    template< class x_r >
    inline void thread::stop( x_r (*p_function)() )
    {
        f_stop = new _callable< _, x_r >( p_function );
    }

}

#endif
