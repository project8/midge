#ifndef _midge_data_hh_
#define _midge_data_hh_

#include "core_message.hh"

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

            void set_time( const real_t& p_time );
            const real_t& get_time() const;

        protected:
            x_raw* f_raw;
            count_t f_size;
            real_t f_interval;
            real_t f_time;

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
            bool start();
            bool execute();
            bool stop();
            void finalize();

        protected:
            typedef enum
            {
                e_idle = 0, e_initialized = 1, e_started = 2
            } state;
            state f_state;

            virtual void initialize_data();
            virtual bool start_data();
            virtual bool execute_data();
            virtual bool stop_data();
            virtual void finalize_data();
    };

    template< class x_type, class x_raw >
    inline _data< x_type, x_raw >::_data() :
            data(),
            f_raw( NULL ),
            f_size( 0 ),
            f_interval( 1. ),
            f_time( 0. ),
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

    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::set_time( const real_t& p_time )
    {
        f_time = p_time;
        return;
    }
    template< class x_type, class x_raw >
    inline const real_t& _data< x_type, x_raw >::get_time() const
    {
        return f_time;
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
            msg_debug( coremsg, "initializing data named <" << this->get_name() << ">" << eom );
            f_state = e_initialized;

            msg_debug( coremsg, "  initializing self" << eom );
            initialize_data();

            msg_debug( coremsg, "initialized data named <" << this->get_name() << ">" << eom );
            return;
        }

        if( f_state == e_initialized )
        {
            msg_debug( coremsg, "already initialized data named <" << this->get_name() << ">" << eom );
            return;
        }

        throw error() << "cannot initialize data named <" << this->get_name() << "> from state <" << f_state << ">";
        return;
    }
    template< class x_type, class x_raw >
    inline bool _data< x_type, x_raw >::start()
    {
        if( f_state == e_initialized )
        {
            msg_debug( coremsg, "starting data named <" << this->get_name() << ">" << eom );
            f_state = e_started;

            msg_debug( coremsg, "  starting self" << eom );
            if( start_data() == false )
            {
                msg_debug( coremsg, "    done" << eom );
                return false;
            }

            msg_debug( coremsg, "  starting outs" << eom );
            for( count_t t_index = 0; t_index < f_outputs.size(); t_index++ )
            {
                msg_debug( coremsg, "  starting out at <" << t_index << ">" << eom );
                if( f_outputs[ t_index ]->start() == false )
                {
                    msg_debug( coremsg, "    done" << eom );
                    return false;
                }
            }

            msg_debug( coremsg, "started data named <" << this->get_name() << ">" << eom );
            return true;
        }

        if( f_state == e_started )
        {
            msg_debug( coremsg, "already started data named <" << this->get_name() << ">" << eom );
            return true;
        }

        return true;
    }
    template< class x_type, class x_raw >
    inline bool _data< x_type, x_raw >::execute()
    {
        if( f_state == e_started )
        {
            msg_debug( coremsg, "executing data named <" << this->get_name() << ">" << eom );

            msg_debug( coremsg, "  executing self" << eom );
            if( execute_data() == false )
            {
                msg_debug( coremsg, "    done" << eom );
                return false;
            }

            msg_debug( coremsg, "  executing outs" << eom );
            for( count_t t_index = 0; t_index < f_outputs.size(); t_index++ )
            {
                msg_debug( coremsg, "  executing out at <" << t_index << ">" << eom );
                if( f_outputs[ t_index ]->execute() == false )
                {
                    msg_debug( coremsg, "    done" << eom );
                    return false;
                }
            }

            msg_debug( coremsg, "executed data named <" << this->get_name() << ">" << eom );
            return true;
        }

        throw error() << "data named <" << get_name() << "> cannot execute from state <" << f_state << ">";
        return false;
    }
    template< class x_type, class x_raw >
    inline bool _data< x_type, x_raw >::stop()
    {
        if( f_state == e_started )
        {
            msg_debug( coremsg, "stopping data named <" << this->get_name() << ">" << eom );
            f_state = e_initialized;

            msg_debug( coremsg, "  stopping self" << eom );
            if( stop_data() == false )
            {
                msg_debug( coremsg, "    done" << eom );
                return false;
            }

            msg_debug( coremsg, "  stopping outs" << eom );
            for( count_t t_index = 0; t_index < f_outputs.size(); t_index++ )
            {
                msg_debug( coremsg, "  stopping out at <" << t_index << ">" << eom );
                if( f_outputs[ t_index ]->stop() == false )
                {
                    msg_debug( coremsg, "    done" << eom );
                    return false;
                }
            }

            msg_debug( coremsg, "stopped data named <" << this->get_name() << ">" << eom );
            return true;
        }

        if( f_state == e_initialized )
        {
            msg_debug( coremsg, "already stopped data named <" << this->get_name() << ">" << eom );
            return false;
        }

        throw error() << "data named <" << get_name() << "> cannot stop from state <" << f_state << ">";
        return false;
    }
    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::finalize()
    {
        if( f_state == e_initialized )
        {
            msg_debug( coremsg, "finalizing data named <" << this->get_name() << ">" << eom );
            f_state = e_idle;

            msg_debug( coremsg, "  finalizing self" << eom );
            finalize_data();

            msg_debug( coremsg, "finalized data named <" << this->get_name() << ">" << eom );
            return;
        }

        if( f_state == e_idle )
        {
            msg_debug( coremsg, "already finalized data named <" << this->get_name() << ">" << eom );
            return;
        }

        throw error() << "data named <" << this->get_name() << "> cannot finalize from state <" << f_state << ">";
        return;
    }

    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::initialize_data()
    {
        return;
    }
    template< class x_type, class x_raw >
    inline bool _data< x_type, x_raw >::start_data()
    {
        return true;
    }
    template< class x_type, class x_raw >
    inline bool _data< x_type, x_raw >::execute_data()
    {
        return true;
    }
    template< class x_type, class x_raw >
    inline bool _data< x_type, x_raw >::stop_data()
    {
        return true;
    }
    template< class x_type, class x_raw >
    inline void _data< x_type, x_raw >::finalize_data()
    {
        return;
    }

}

#endif
