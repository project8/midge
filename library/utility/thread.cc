#include "../library/utility/thread.hh"

namespace midge
{
    thread::thread() :
            f_thread(),
            f_thread_mutex(),
            f_state( e_ready ),
            f_state_mutex(),
            f_start( NULL ),
            f_stop( NULL )
    {
    }
    thread::~thread()
    {
        if( get_state() == e_executing )
        {
            pthread_cancel( f_thread );
        }
    }

    void thread::start()
    {
        if( get_state() == e_ready )
        {
            f_thread_mutex.lock();
            pthread_create( &f_thread, 0, &thread::thread_start, this );
            f_thread_mutex.lock();
            f_thread_mutex.unlock();
        }
        return;
    }
    void thread::join()
    {
        if( get_state() == e_executing )
        {
            f_thread_mutex.lock();
            pthread_join( f_thread, 0 );
            f_thread_mutex.unlock();
        }
        return;
    }
    void thread::stop()
    {
        if( get_state() == e_executing )
        {
            f_thread_mutex.lock();
            pthread_cancel( f_thread );
            f_thread_mutex.lock();
            f_thread_mutex.unlock();
        }
        return;
    }

    thread::state thread::get_state()
    {
        thread::state t_state;
        f_state_mutex.lock();
        t_state = f_state;
        f_state_mutex.unlock();
        return t_state;
    }

    void* thread::thread_start( void* t_thread_ptr )
    {
        thread* t_thread = reinterpret_cast< ::midge::thread* >( t_thread_ptr );
        t_thread->f_state_mutex.lock();
        t_thread->f_state = ::midge::thread::e_executing;
        t_thread->f_state_mutex.unlock();
        t_thread->f_thread_mutex.unlock();
        if( t_thread->f_start != NULL )
        {
            pthread_cleanup_push( &::midge::thread::thread_stop, t_thread_ptr );
            t_thread->f_start->execute();
            pthread_cleanup_pop( 0 );
        }
        t_thread->f_state_mutex.lock();
        t_thread->f_state = ::midge::thread::e_executed;
        t_thread->f_state_mutex.unlock();
        return t_thread_ptr;
    }

    void thread::thread_stop( void* t_thread_ptr )
    {
        thread* t_thread = reinterpret_cast< ::midge::thread* >( t_thread_ptr );
        t_thread->f_state_mutex.lock();
        t_thread->f_state = ::midge::thread::e_cancelling;
        t_thread->f_state_mutex.unlock();
        t_thread->f_thread_mutex.unlock();
        if( t_thread->f_stop != NULL )
        {
            t_thread->f_stop->execute();
        }
        t_thread->f_state_mutex.lock();
        t_thread->f_state = ::midge::thread::e_cancelled;
        t_thread->f_state_mutex.unlock();
        return;
    }

}
