#ifndef _midge_pool_hh_
#define _midge_pool_hh_

#include <stack>

#include "mutex.hh"
#include "pointer.hh"
using std::stack;

namespace midge
{
    using scarab::mutex;

    template< class x_type >
    class pool
    {
        public:
            static void initialize( const count_t& p_size );
            static count_t count();
            static void finalize();

            static x_type* allocate();
            static void free( x_type* p_pointer );

        private:
            static mutex s_mutex;
            static stack< x_type* > s_stack;
    };

    template< class x_type >
    mutex pool< x_type >::s_mutex = mutex();

    template< class x_type >
    stack< x_type* > pool< x_type >::s_stack = stack< x_type* >();

    template< class x_type >
    void pool< x_type >::initialize( const count_t& p_size )
    {
        s_mutex.lock();
        for( index_t t_index = 0; t_index < p_size; t_index++ )
        {
            s_stack.push( new x_type() );
        }
        s_mutex.unlock();
        return;
    }
    template< class x_type >
    count_t pool< x_type >::count()
    {
        count_t t_count;
        s_mutex.lock();
        t_count = s_stack.size();
        s_mutex.unlock();
        return t_count;
    }
    template< class x_type >
    void pool< x_type >::finalize()
    {
        s_mutex.lock();
        while( s_stack.empty() == false )
        {
            delete s_stack.top();
            s_stack.pop();
        }
        s_mutex.unlock();
        return;
    }

    template< class x_type >
    x_type* pool< x_type >::allocate()
    {
        x_type* t_pointer;
        s_mutex.lock();
        if( s_stack.size() > 0 )
        {
            t_pointer = s_stack.top();
            s_stack.pop();
        }
        else
        {
            t_pointer = NULL;
            throw error() << "pool< " << typeid(x_type).name() << " > is empty";
        }
        s_mutex.unlock();
        return t_pointer;
    }
    template< class x_type >
    void pool< x_type >::free( x_type* p_pointer )
    {
        s_mutex.lock();
        s_stack.push( p_pointer );
        s_mutex.unlock();
        return;
    }

}

#endif
