#ifndef _midge_pointer_hh_
#define _midge_pointer_hh_

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
                    int increment()
                    {
                        return ++f_count;
                    }
                    int decrement()
                    {
                        return --f_count;
                    }
                private:
                    int f_count;
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
