#ifndef _midge_singleton_hh_
#define _midge_singleton_hh_

namespace midge
{

    template< class x_type >
    class singleton
    {
        public:
            static x_type* get_instance();
            static void create_instance();
            static void delete_instance();

        private:
            static x_type* f_instance;

        protected:
            singleton()
            {
            }
            virtual ~singleton()
            {
            }
    };

    template< class x_type >
    x_type* singleton< x_type >::f_instance( 0 );

    template< class x_type >
    x_type* singleton< x_type >::get_instance()
    {
        if( f_instance == 0 )
        {
            create_instance();
        }
        return f_instance;
    }

    template< class x_type >
    void singleton< x_type >::create_instance()
    {
        if( !f_instance )
        {
            f_instance = new x_type();
        }
    }

    template< class x_type >
    void singleton< x_type >::delete_instance()
    {
        if( f_instance != 0 )
        {
            delete f_instance;
            f_instance = 0;
        }
    }

}

#endif
