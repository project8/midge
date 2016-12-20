#include "thread.hh"

namespace midge
{
    thread::thread() :
            f_thread(),
            f_thread_mutex(),
            f_state( e_ready ),
            f_canceled( new std::atomic< bool >( false ) ),
            f_start( new callable() ),
            f_stop( new callable() )
    {
    }
    thread::~thread()
    {
        if( get_state() == e_executing )
        {
            f_canceled->store( true );
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
        if( get_state() == e_executing )
        {
            f_thread.join();
        }
        return;
    }
    void thread::stop()
    {
        if( get_state() == e_executing )
        {
            std::unique_lock< std::mutex >( f_thread_mutex );
            f_state.store( e_cancelling );
            f_stop->execute();
            f_canceled->store( true );
            f_state.store( e_cancelled );
        }
        return;
    }

    void thread::thread_start()
    {
        f_state.store( e_executing );
        f_start->execute();
        stop();
        f_state.store( e_executed );
       return;
    }

}
