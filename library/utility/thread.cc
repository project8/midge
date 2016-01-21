#include "thread.hh"

namespace midge
{
    thread::thread() :
            f_thread(),
            f_thread_mutex(),
            f_state( e_ready ),
            f_start( nullptr ),
            f_stop( nullptr )
    {
    }
    thread::~thread()
    {
        if( get_state() == e_executing )
        {
            f_canceled.store( true );
        }
    }

    void thread::start()
    {
        if( get_state() == e_ready )
        {
            std::unique_lock< std::mutex >( f_thread_mutex );
            f_thread = std::thread( [this](){ thread_start(); } );
        }
        return;
    }
    void thread::join()
    {
        if( f_state.load() == e_executing )
        {
            std::unique_lock< std::mutex >( f_thread_mutex );
            f_thread.join();
        }
        return;
    }
    void thread::stop()
    {
        if( f_state.load() == e_executing )
        {
            std::unique_lock< std::mutex >( f_thread_mutex );
            f_canceled.store( true );
        }
        return;
    }

    thread::state thread::get_state()
    {
        return f_state.load();
    }

    void thread::thread_start()
    {
        f_state.store( e_executing );
        if( f_start != nullptr )
        {
            f_start->execute();
            thread_stop();
        }
        f_state.store( e_executed );
       return;
    }

    void thread::thread_stop()
    {
        f_state.store( e_cancelling );
        if( f_stop != nullptr )
        {
            f_stop->execute();
        }
        f_state.store( e_cancelled );
        return;
    }

}
