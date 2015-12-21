#ifndef _midge_value_hh_
#define _midge_value_hh_

#include <string>

#include "../utility/error.hh"
#include "../utility/types.hh"
using std::string;

namespace midge
{

    class value
    {
        private:

        public:
            value();
            virtual ~value();
            virtual value* clone() const = 0;

        public:
            template< class x_type >
            x_type* as();

            template< class x_type >
            const x_type* as() const;

            template< class x_type >
            bool is() const;

        protected:
            template< class x_type >
            void set( x_type* p_type );

        private:
            class base
            {
                public:
                    base();
                    virtual ~base();

                public:
                    virtual void open() const = 0;
            };

            template< class x_type >
            class holder :
                public base
            {
                public:
                    holder( x_type* p_type );
                    virtual ~holder();

                public:
                    void open() const;

                private:
                    x_type* f_type;
            };

            base* f_base;
    };

    inline value::base::base()
    {
    }
    inline value::base::~base()
    {
    }

    template< class x_type >
    inline value::holder< x_type >::holder( x_type* p_type ) :
            base(),
            f_type( p_type )
    {
    }
    template< class x_type >
    inline value::holder< x_type >::~holder()
    {
    }
    template< class x_type >
    inline void value::holder< x_type >::open() const
    {
        throw f_type;
    }

    template< class x_type >
    inline x_type* value::as()
    {
        if( f_base != NULL )
        {
            try
            {
                f_base->open();
            }
            catch( x_type* t_type )
            {
                return t_type;
            }
            catch( ... )
            {
            }
        }

        throw error() << "cannot resolve value to type <" << typeid(x_type).name() << ">";
        return NULL;
    }

    template< class x_type >
    inline const x_type* value::as() const
    {
        if( f_base != NULL )
        {
            try
            {
                f_base->open();
            }
            catch( x_type* t_type )
            {
                return t_type;
            }
            catch( ... )
            {
            }
        }

        throw error() << "cannot resolve value to type <" << typeid(x_type).name() << ">";
        return NULL;
    }

    template< class x_type >
    inline bool value::is() const
    {
        if( f_base != NULL )
        {

            try
            {
                f_base->open();
            }
            catch( x_type* )
            {
                return true;
            }
            catch( ... )
            {
            }
        }
        return false;
    }

    template< class x_type >
    inline void value::set( x_type* p_type )
    {
        delete f_base;
        f_base = new holder< x_type >( p_type );
        return;
    }

}

#endif
