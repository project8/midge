#ifndef _midge_operand_hh_
#define _midge_operand_hh_

#include "component.hh"

#include <vector>
using std::vector;

namespace midge
{

    template< class x_type, class x_data >
    class operand :
        public component
    {
        public:
            operand();
            virtual ~operand();

            //****
            //data
            //****

        public:
            void set_size( const uint64_t& p_size );
            const uint64_t& get_size() const;

            x_data* data();
            const x_data* data() const;

        protected:
            uint64_t f_size;
            x_data* f_data;

            //***********
            //composition
            //***********

        public:
            void set_input( node* p_input );
            void add_output( node* p_output );

        protected:
            node* f_input;
            vector< node* > f_outputs;

            //******
            //action
            //******

        public:
            void initialize();
            void finalize();
            void execute();

        protected:
            typedef enum
            {
                e_idle = 0, e_initialized = 1
            } state;
            state f_state;

            virtual void initialize_operand();
            virtual void execute_operand();
            virtual void finalize_operand();
    };

    template< class x_type, class x_data >
    inline operand< x_type, x_data >::operand() :
            f_size( 0 ),
            f_data( NULL ),
            f_input( NULL ),
            f_outputs(),
            f_state( e_idle )
    {
        input( this, &operand< x_type, x_data >::set_input, "set_input" );
        output( this, &operand< x_type, x_data >::add_output, "add_output" );
    }
    template< class x_type, class x_data >
    inline operand< x_type, x_data >::~operand()
    {
    }

    //****
    //data
    //****

    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::set_size( const uint64_t& p_size )
    {
        f_size = p_size;
        return;
    }
    template< class x_type, class x_data >
    inline const uint64_t& operand< x_type, x_data >::get_size() const
    {
        return f_size;
    }

    template< class x_type, class x_data >
    inline x_data* operand< x_type, x_data >::data()
    {
        return f_data;
    }

    //***********
    //composition
    //***********

    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::set_input( node* p_input )
    {
        f_input = p_input;
        return;
    }

    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::add_output( node* p_output )
    {
        f_outputs.push_back( p_output );
        return;
    }

    //******
    //action
    //******

    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::initialize()
    {
        if( f_state == e_idle )
        {
            f_state = e_initialized;

            f_data = new x_data[ f_size ];

            initialize_operand();

            for( typename vector< node* >::iterator t_it = f_outputs.begin(); t_it != f_outputs.end(); t_it++ )
            {
                (*t_it)->initialize();
            }
        }

        if( f_state != e_initialized )
        {
            throw error() << "operand named <" << get_name() << "> cannot initialize from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::execute()
    {
        if( f_state == e_initialized )
        {
            execute_operand();

            for( typename vector< node* >::iterator t_it = f_outputs.begin(); t_it != f_outputs.end(); t_it++ )
            {
                (*t_it)->execute();
            }
        }
        else
        {
            throw error() << "operand named <" << get_name() << "> cannot execute from state <" << f_state << ">";
        }

        return;
    }
    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::finalize()
    {
        if( f_state == e_initialized )
        {
            f_state = e_idle;

            finalize_operand();

            delete[] f_data;

            for( typename vector< node* >::iterator t_it = f_outputs.begin(); t_it != f_outputs.end(); t_it++ )
            {
                (*t_it)->finalize();
            }
        }

        if( f_state != e_idle )
        {
            throw error() << "operand named <" << get_name() << "> cannot finalize from state <" << f_state << ">";
        }

        return;
    }

    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::initialize_operand()
    {
        return;
    }
    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::execute_operand()
    {
        return;
    }
    template< class x_type, class x_data >
    inline void operand< x_type, x_data >::finalize_operand()
    {
        return;
    }

}

#endif
