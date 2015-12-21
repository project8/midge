#ifndef _midge_pointer_hh_
#define _midge_pointer_hh_

#include <cstddef>
#include "error.hh"
#include "mutex.hh"
#include "pool.hh"
#include "types.hh"

namespace midge
{

    template< typename x_type >
    class pointer
    {
        private:
            class counter
            {
                public:
                    counter() :
                            f_count( 0 )
                    {
                    }
                    ~counter()
                    {
                    }

                public:
                    count_t count() const
                    {
                        count_t t_count = f_count;
                        return t_count;
                    }
                    count_t increment()
                    {
                        count_t t_count = ++f_count;
                        return t_count;
                    }
                    count_t decrement()
                    {
                        count_t t_count = --f_count;
                        return t_count;
                    }

                private:
                    count_t f_count;
            };

        public:
            pointer() :
                    f_pointer( NULL ),
                    f_counter( pool< counter >::allocate() )
            {
                f_counter->increment();
            }

            pointer( x_type* p_pointer ) :
                    f_pointer( p_pointer ),
                    f_counter( pool< counter >::allocate() )
            {
                f_counter->increment();
            }

            pointer< x_type >& operator=( x_type* p_pointer )
            {
                if( f_pointer != p_pointer )
                {
                    if( f_counter->decrement() == 0 )
                    {
                        if( f_pointer != NULL )
                        {
                            pool< x_type >::free( f_pointer );
                        }
                        pool< counter >::free( f_counter );
                    }

                    f_pointer = p_pointer;
                    f_counter = pool< counter >::allocate();
                    f_counter->increment();
                }
                return *this;
            }

            pointer( const pointer< x_type >& p_pointer ) :
                    f_pointer( p_pointer.f_pointer ),
                    f_counter( p_pointer.f_counter )
            {
                f_counter->increment();
            }

            pointer< x_type >& operator=( const pointer< x_type >& p_pointer )
            {
                if( this != &p_pointer )
                {
                    if( f_counter->decrement() == 0 )
                    {
                        if( f_pointer != NULL )
                        {
                            pool< x_type >::free( f_pointer );
                        }
                        pool< counter >::free( f_counter );
                    }

                    f_pointer = p_pointer.f_pointer;
                    f_counter = p_pointer.f_counter;
                    f_counter->increment();
                }
                return *this;
            }

            ~pointer()
            {
                if( f_counter->decrement() == 0 )
                {
                    if( f_pointer != NULL )
                    {
                        pool< x_type >::free( f_pointer );
                    }
                    pool< counter >::free( f_counter );
                }
            }

            static void initialize( const count_t& p_size )
            {
                pool< counter >::initialize( p_size );
                return;
            }
            static count_t count()
            {
                return pool< counter >::count();
            }
            static void finalize()
            {
                pool< counter >::finalize();
                return;
            }

            void allocate()
            {
                if( f_counter->decrement() == 0 )
                {
                    if( f_pointer != NULL )
                    {
                        pool< x_type >::free( f_pointer );
                    }
                    pool< counter >::free( f_counter );
                }
                f_pointer = pool< x_type >::allocate();
                f_counter = pool< counter >::allocate();
                return;
            }

            void free()
            {
                if( f_counter->decrement() == 0 )
                {
                    if( f_pointer != NULL )
                    {
                        pool< x_type >::free( f_pointer );
                    }
                    pool< counter >::free( f_counter );
                }
                f_pointer = NULL;
                f_counter = pool< counter >::allocate();
                return;
            }

            x_type& operator*()
            {
                if( f_pointer == NULL )
                {
                    throw error() << "pointer of type <" << typeid(x_type).name() << "> tried to dereference null pointer with *";
                }
                return *f_pointer;
            }

            const x_type& operator*() const
            {
                if( f_pointer == NULL )
                {
                    throw error() << "pointer of type <" << typeid(x_type).name() << "> tried to dereference null pointer with * (const)";
                }
                return *f_pointer;
            }

            x_type* operator->()
            {
                if( f_pointer == NULL )
                {
                    throw error() << "pointer of type <" << typeid(x_type).name() << "> tried to dereference null pointer with ->";
                }
                return f_pointer;
            }

            const x_type* operator->() const
            {
                if( f_pointer == NULL )
                {
                    throw error() << "pointer of type <" << typeid(x_type).name() << "> tried to dereference null pointer with -> (const)";
                }
                return f_pointer;
            }

            bool null() const
            {
                return f_pointer == NULL;
            }
            count_t count() const
            {
                return f_counter->count();
            }

            bool operator==( const pointer< x_type >& sp ) const
            {
                return f_pointer == sp.f_pointer;
            }
            bool operator<( const pointer< x_type >& sp ) const
            {
                return f_pointer < sp.f_pointer;
            }
            bool operator>( const pointer< x_type >& sp ) const
            {
                return f_pointer > sp.f_pointer;
            }

        private:
            x_type* f_pointer;
            counter* f_counter;
            mutable mutex f_mutex;
    };
}

#endif
