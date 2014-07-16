#ifndef _midge_object_hh_
#define _midge_object_hh_

#include <cstddef>

namespace midge
{

    class object
    {
        private:
            class holder
            {
                public:
                    holder();
                    virtual ~holder();

                public:
                    virtual void type() = 0;
            };

        private:
            template< class x_type >
            class holder_template :
                public holder
            {
                public:
                    holder_template( x_type* p_object );
                    virtual ~holder_template();

                public:
                    void type();

                private:
                    x_type* f_object;
            };

        public:
            object();
            virtual ~object();

        public:
            template< class x_type >
            void set( x_type* p_object );

            template< class x_type >
            x_type* as();

            template< class x_type >
            const x_type* as() const;

        private:
            holder* f_holder;
    };

    object::holder::holder()
    {
    }
    object::holder::~holder()
    {
    }

    template< class x_type >
    object::holder_template< x_type >::holder_template( x_type* p_object ) :
            f_object( p_object )
    {
    }
    template< class x_type >
    object::holder_template< x_type >::~holder_template()
    {
    }
    template< class x_type >
    void object::holder_template< x_type >::type()
    {
        throw f_object;
        return;
    }

    template< class x_type >
    void object::set( x_type* p_object )
    {
        delete f_holder;
        f_holder = new holder_template< x_type >( p_object );
        return;
    }

    template< class x_type >
    x_type* object::as()
    {
        if( f_holder != NULL )
        {
            try
            {
                f_holder->type();
            }
            catch( x_type* t_object )
            {
                return t_object;
            }
        }
        return NULL;
    }

    template< class x_type >
    const x_type* object::as() const
    {
        if( f_holder != NULL )
        {
            try
            {
                f_holder->type();
            }
            catch( x_type* t_object )
            {
                return t_object;
            }
        }
        return NULL;
    }

}

#endif
