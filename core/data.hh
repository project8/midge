#ifndef _midge_data_hh_
#define _midge_data_hh_

#include "node.hh"

#include <vector>
using std::vector;

namespace midge
{

    class data :
        virtual public node
    {
        protected:
            data();

        public:
            virtual ~data();
    };

    template< class x_type, class x_raw >
    class _data :
        public data
    {
        public:
            _data();
            virtual ~_data();

            //****
            //data
            //****

        public:
            x_raw* raw();
            const x_raw* raw() const;

            void set_size( const count_t& p_size );
            const count_t& get_size() const;

            void set_interval( const real_t& p_interval );
            const real_t& get_interval() const;

        protected:
            x_raw* f_raw;
            count_t f_size;
            real_t f_interval;

            //***********
            //composition
            //***********

        public:
            void in( node* p_input );
            void out( node* p_output );

        protected:
            node* f_input;
            vector< node* > f_outputs;

            //******
            //action
            //******

        public:
            void initialize();
            void start();
            void execute();
            void stop();
            void finalize();

        protected:
            typedef enum
            {
                e_idle = 0, e_initialized = 1, e_started = 2
            } state;
            state f_state;

            virtual void initialize_data();
            virtual void start_data();
            virtual void execute_data();
            virtual void stop_data();
            virtual void finalize_data();
    };

    template< class x_type, class x_raw >
    inline _data< x_type, x_raw >::_data() :
            data(),
            f_raw( NULL ),
            f_size( 0 ),
            f_interval( 1. ),
            f_input( NULL ),
            f_outputs(),
            f_state( e_idle )
    {
        node::in( &_data< x_type, x_raw >::in, "in" );
        node::out( &_data< x_type, x_raw >::out, "out" );
    }
    template< class x_type, class x_raw >
    inline _data< x_type, x_raw >::~_data()
    {
    }

    //****
    //data
    //****

    template< class x_type, class x_raw >
    inline x_raw* _data< x_type, x_raw >::raw()
    {
        return f_raw;
    }

    template< class x_type, class x_raw >
    inline const x_raw* _data< x_type, x_raw >::raw() const
    {
        return f_raw;
    }

    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::set_size( const count_t& p_size )
    {
        f_size = p_size;

        if( f_raw != NULL )
        {
            free( f_raw );
            f_raw = NULL;
        }

        if( f_raw == NULL )
        {
            f_raw = (x_raw*) (malloc( sizeof(x_raw) * f_size ));
        }

        return;
    }
    template< class x_type, class x_raw >
    inline const count_t& _data< x_type, x_raw >::get_size() const
    {
        return f_size;
    }

    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::set_interval( const real_t& p_interval )
    {
        f_interval = p_interval;
        return;
    }
    template< class x_type, class x_raw >
    inline const real_t& _data< x_type, x_raw >::get_interval() const
    {
        return f_interval;
    }

    //***********
    //composition
    //***********

    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::in( node* p_input )
    {
        f_input = p_input;
        return;
    }

    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::out( node* p_output )
    {
        f_outputs.push_back( p_output );
        return;
    }

    //******
    //action
    //******

    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::initialize()
    {
        if( f_state == e_idle )
        {
            f_state = e_initialized;

            initialize_data();
        }

        if( f_state != e_initialized )
        {
            throw error() << "data named <" << get_name() << "> cannot initialize from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::start()
    {
        if( f_state == e_initialized )
        {
            f_state = e_started;

            start_data();

            for( typename vector< node* >::iterator t_it = f_outputs.begin(); t_it != f_outputs.end(); t_it++ )
            {
                (*t_it)->start();
            }
        }

        if( f_state != e_started )
        {
            throw error() << "data named <" << get_name() << "> cannot start from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::execute()
    {
        if( f_state == e_started )
        {
            execute_data();

            for( typename vector< node* >::iterator t_it = f_outputs.begin(); t_it != f_outputs.end(); t_it++ )
            {
                (*t_it)->execute();
            }
        }
        else
        {
            throw error() << "data named <" << get_name() << "> cannot execute from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::stop()
    {
        if( f_state == e_started )
        {
            f_state = e_initialized;

            stop_data();

            for( typename vector< node* >::iterator t_it = f_outputs.begin(); t_it != f_outputs.end(); t_it++ )
            {
                (*t_it)->stop();
            }
        }

        if( f_state != e_initialized )
        {
            throw error() << "data named <" << get_name() << "> cannot stop from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::finalize()
    {
        if( f_state == e_initialized )
        {
            f_state = e_idle;

            finalize_data();
        }

        if( f_state != e_idle )
        {
            throw error() << "data named <" << get_name() << "> cannot finalize from state <" << f_state << ">";
        }

        return;
    }

    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::initialize_data()
    {
        return;
    }
    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::start_data()
    {
        return;
    }
    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::execute_data()
    {
        return;
    }
    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::stop_data()
    {
        return;
    }
    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::finalize_data()
    {
        return;
    }

}

#endif
