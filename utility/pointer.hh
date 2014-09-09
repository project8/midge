#ifndef _midge_pointer_hh_
#define _midge_pointer_hh_

#include "types.hh"

#include <cstddef>

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
                    const count_t& count() const
                    {
                        return f_count;
                    }
                    const count_t& increment()
                    {
                        return ++f_count;
                    }
                    const count_t& decrement()
                    {
                        return --f_count;
                    }
                private:
                    count_t f_count;
            };

        public:
            pointer() :
                    f_pointer( NULL ),
                    f_counter( new counter() )
            {
                f_counter->increment();
            }
            pointer( x_type* p_value ) :
                    f_pointer( p_value ),
                    f_counter( new counter() )
            {
                f_counter->increment();
            }

            pointer( const pointer< x_type >& sp ) :
                    f_pointer( sp.f_pointer ),
                    f_counter( sp.f_counter )
            {
                f_counter->increment();
            }

            ~pointer()
            {
                if( f_counter->decrement() == 0 )
                {
                    delete f_pointer;
                    delete f_counter;
                }
            }

            x_type& operator*()
            {
                return *f_pointer;
            }

            const x_type& operator*() const
            {
                return *f_pointer;
            }

            x_type* operator->()
            {
                return f_pointer;
            }

            const x_type* operator->() const
            {
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

            pointer< x_type >& operator=( const pointer< x_type >& sp )
            {
                if( this != &sp )
                {
                    if( f_counter->decrement() == 0 )
                    {
                        delete f_pointer;
                        delete f_counter;
                    }

                    f_pointer = sp.f_pointer;
                    f_counter = sp.f_counter;
                    f_counter->increment();
                }
                return *this;
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
    };
}



#endif
