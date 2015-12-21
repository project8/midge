#ifndef _midge_singleton_hh_
#define _midge_singleton_hh_

#include "../library/utility/mutex.hh"

namespace midge
{

    template< class x_type >
    class singleton
    {
        public:
            static x_type* get_instance();
            static void kill_instance();

        private:
            static void create_instance();
            static void delete_instance();

        private:
            static x_type* f_instance;
            static mutex f_mutex;

        protected:
            singleton()
            {
            }
            virtual ~singleton()
            {
            }
    };

    template< class x_type >
    x_type* singleton< x_type >::f_instance( NULL );

    template< class x_type >
    mutex singleton< x_type >::f_mutex;

    template< class x_type >
    x_type* singleton< x_type >::get_instance()
    {
        if( f_instance == NULL )
        {
            f_mutex.lock();
            create_instance();
            f_mutex.unlock();
        }
        return f_instance;
    }
    template< class x_type >
    void singleton< x_type >::kill_instance()
    {
        if( f_instance != NULL )
        {
            f_mutex.lock();
            delete_instance();
            f_mutex.unlock();
        }
        return;
    }

    template< class x_type >
    void singleton< x_type >::create_instance()
    {
        if( f_instance == NULL )
        {
            f_instance = new x_type();
        }
    }

    template< class x_type >
    void singleton< x_type >::delete_instance()
    {
        if( f_instance != NULL )
        {
            delete f_instance;
            f_instance = NULL;
        }
    }

}

#endif
