#ifndef _midge_pool_hh_
#define _midge_pool_hh_

#include "pointer.hh"
#include "mutex.hh"

#include <stack>
using std::stack;

namespace midge
{

    template< class x_type >
    class pool
    {
        public:
            static void initialize( const count_t& p_size );
            static void finalize();

            static void allocate( pointer< x_type >& p_data );

            template< template< class > class x_container >
            static void allocate( x_container< pointer< x_type > >& p_data );

            template< template< class, class > class x_container, class x_1 >
            static void allocate( x_container< pointer< x_type >, x_1 >& p_data );

            template< template< class, class, class > class x_container, class x_1, class x_2 >
            static void allocate( x_container< pointer< x_type >, x_1, x_2 >& p_data );

            static void free( pointer< x_type >& p_data );

            template< template< class > class x_container >
            static void free( x_container< pointer< x_type > >& p_data );

            template< template< class, class > class x_container, class x_1 >
            static void free( x_container< pointer< x_type >, x_1 >& p_data );

            template< template< class, class, class > class x_container, class x_1, class x_2 >
            static void free( x_container< pointer< x_type >, x_1, x_2 >& p_data );

        private:
            static mutex s_mutex;
            static stack< pointer< x_type > > s_stack;
    };

    template< class x_type >
    mutex pool< x_type >::s_mutex = mutex();

    template< class x_type >
    stack< pointer< x_type > > pool< x_type >::s_stack = stack< pointer< x_type > >();

    template< class x_type >
    void pool< x_type >::initialize( const count_t& p_size )
    {
        s_mutex.lock();
        for( index_t t_index = 0; t_index < p_size; t_index++ )
        {
            s_stack.push( pointer< x_type >( new x_type() ) );
        }
        s_mutex.unlock();
        return;
    }
    template< class x_type >
    void pool< x_type >::finalize()
    {
        s_mutex.lock();
        while( s_stack.empty() == false )
        {
            s_stack.pop();
        }
        s_mutex.unlock();
        return;
    }

    template< class x_type >
    void pool< x_type >::allocate( pointer< x_type >& p_data )
    {
        s_mutex.lock();
        if( s_stack.size() > 0 )
        {
            p_data = s_stack.top();
            s_stack.pop();
        }
        else
        {
            p_data = new x_type();
        }
        s_mutex.unlock();
        return;
    }

    template< class x_type >
    template< template< class > class x_container >
    void pool< x_type >::allocate( x_container< pointer< x_type > >& p_data )
    {
        s_mutex.lock();
        typename x_container< pointer< x_type > >::iterator t_it = p_data.begin();
        while( (s_stack.empty() == false) && (t_it == p_data.end()) )
        {
            (*t_it) = s_stack.top();
            ++t_it;
            s_stack.pop();
        }
        while( t_it != p_data.end() )
        {
            (*t_it) = new x_type();
            ++t_it;
        }
        s_mutex.unlock();
        return;
    }

    template< class x_type >
    template< template< class, class > class x_container, class x_1 >
    void pool< x_type >::allocate( x_container< pointer< x_type >, x_1 >& p_data )
    {
        s_mutex.lock();
        typename x_container< pointer< x_type >, x_1 >::iterator t_it = p_data.begin();
        while( (s_stack.empty() == false) && (t_it == p_data.end()) )
        {
            (*t_it) = s_stack.top();
            ++t_it;
            s_stack.pop();
        }
        while( t_it != p_data.end() )
        {
            (*t_it) = new x_type();
            ++t_it;
        }
        s_mutex.unlock();
        return;
    }

    template< class x_type >
    template< template< class, class, class > class x_container, class x_1, class x_2 >
    void pool< x_type >::allocate( x_container< pointer< x_type >, x_1, x_2 >& p_data )
    {
        s_mutex.lock();
        typename x_container< pointer< x_type >, x_1, x_2 >::iterator t_it = p_data.begin();
        while( (s_stack.empty() == false) && (t_it == p_data.end()) )
        {
            (*t_it) = s_stack.top();
            ++t_it;
            s_stack.pop();
        }
        while( t_it != p_data.end() )
        {
            (*t_it) = new x_type();
            ++t_it;
        }
        s_mutex.unlock();
        return;
    }

    template< class x_type >
    void pool< x_type >::free( pointer< x_type >& p_data )
    {
        s_mutex.lock();
        s_stack.push( p_data );
        p_data = NULL;
        s_mutex.unlock();
        return;
    }

    template< class x_type >
    template< template< class > class x_container >
    void pool< x_type >::free( x_container< pointer< x_type > >& p_data )
    {
        s_mutex.lock();
        typename x_container< pointer< x_type > >::iterator t_it = p_data.begin();
        while( t_it != p_data.end() )
        {
            s_stack.push( *t_it );
            (*t_it) = NULL;
            t_it++;
        }
        s_mutex.unlock();
        return;
    }

    template< class x_type >
    template< template< class, class > class x_container, class x_1 >
    void pool< x_type >::free( x_container< pointer< x_type >, x_1 >& p_data )
    {
        s_mutex.lock();
        typename x_container< pointer< x_type >, x_1 >::iterator t_it = p_data.begin();
        while( t_it != p_data.end() )
        {
            s_stack.push( *t_it );
            (*t_it) = NULL;
            t_it++;
        }
        s_mutex.unlock();
        return;
    }

    template< class x_type >
    template< template< class, class, class > class x_container, class x_1, class x_2 >
    void pool< x_type >::free( x_container< pointer< x_type >, x_1, x_2 >& p_data )
    {
        s_mutex.lock();
        typename x_container< pointer< x_type >, x_1, x_2 >::iterator t_it = p_data.begin();
        while( t_it != p_data.end() )
        {
            s_stack.push( *t_it );
            (*t_it) = NULL;
            t_it++;
        }
        s_mutex.unlock();
        return;
    }

}

#endif
