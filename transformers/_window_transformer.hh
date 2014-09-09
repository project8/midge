#ifndef _midge__window_transformer_hh_
#define _midge__window_transformer_hh_

#include "transformer.hh"
#include "_t_data.hh"
#include "_f_data.hh"
#include "_tf_data.hh"
#include "types.hh"
#include "typelist.hh"
#include "macros.hh"
#include "window.hh"

#include <cmath>

namespace midge
{

    template< class x_type >
    class _window_transformer :
        public _transformer< _window_transformer< x_type >, typelist_1( x_type ),typelist_1( x_type ) >
    {
        public:
            typedef _transformer< _window_transformer< x_type >, typelist_1( x_type ),typelist_1( x_type ) > parent;

        public:
            _window_transformer();
            virtual ~_window_transformer();

        public:
            assignable( window, window )
            accessible( count_t, length )

        protected:
            void initialize();
            void execute();
            void finalize();

        private:
            template< class x_header_type >
            class _header;

            template< class x_data_type >
            class _data;
    };

    template< class x_type >
    _window_transformer< x_type >::_window_transformer() :
            f_window( NULL ),
            f_length( 10 )
    {
    }
    template< class x_type >
        _window_transformer< x_type >::~_window_transformer()
    {
    }

    template< class x_type >
    void _window_transformer< x_type >::initialize()
    {
        parent::template out_buffer< 0 >().initialize( f_length );

        if( f_window == NULL )
        {
            throw error() << "window transformer window unset";
        }

        return;
    }

    template< class x_type >
    void _window_transformer< x_type >::execute()
    {
        enum_t t_in_command;
        const x_type* t_in_data;
        x_type* t_out_data;

        _header< x_type > t_header;
        _data< x_type > t_data;

        while( true )
        {
            t_in_command = parent::template in_stream< 0 >().get();
            t_in_data = parent::template in_stream< 0 >().data();

            t_out_data = parent::template out_stream< 0 >().data();

            if( t_in_command == stream::s_start )
            {
                t_header.copy( t_in_data, t_out_data, f_window );

                parent::template out_stream< 0 >().set( stream::s_start );
                continue;
            }
            if( t_in_command == stream::s_run )
            {
                t_header.copy( t_in_data, t_out_data, f_window );
                t_data.window( t_in_data, t_out_data, f_window );

                parent::template out_stream< 0 >().set( stream::s_run );
                continue;
            }
            if( t_in_command == stream::s_stop )
            {
                t_header.copy( t_in_data, t_out_data, f_window );

                parent::template out_stream< 0 >().set( stream::s_stop );
                continue;
            }
            if( t_in_command == stream::s_exit )
            {
                t_header.copy( t_in_data, t_out_data, f_window );

                parent::template out_stream< 0 >().set( stream::s_exit );
                return;
            }
        }

        return;
    }

    template< class x_type >
    void _window_transformer< x_type >::finalize()
    {
        parent::template out_buffer< 0 >().finalize();

        delete f_window;

        return;
    }


    template< class x_type >
    template< class x_header_type >
    class _window_transformer< x_type >::_header< _t_data< x_header_type > >
    {
        public:
            inline void copy( const _t_data< x_header_type >* p_from, _t_data< x_header_type >* p_to, window* p_window )
            {
                p_to->set_size( p_from->get_size() );
                p_to->set_time_interval( p_from->get_time_interval() );
                p_to->set_time_index( p_from->get_time_index() );
                p_window->set_size( p_from->get_size() );
                return;
            }
    };

    template< class x_type >
    template< class x_header_type >
    class _window_transformer< x_type >::_header< _f_data< x_header_type > >
    {
        public:
            inline void copy( const _f_data< x_header_type >* p_from, _f_data< x_header_type >* p_to, window* p_window )
            {
                p_to->set_size( p_from->get_size() );
                p_to->set_frequency_interval( p_from->get_frequency_interval() );
                p_to->set_frequency_index( p_from->get_frequency_index() );
                p_window->set_size( p_from->get_size() );
                return;
            }
    };

    template< class x_type >
    template< class x_header_type >
    class _window_transformer< x_type >::_header< _tf_data< x_header_type > >
    {
        public:
            inline void copy( const _tf_data< x_header_type >* p_from, _tf_data< x_header_type >* p_to, window* p_window )
            {
                p_to->set_size( p_from->get_size() );
                p_to->set_time_interval( p_from->get_time_interval() );
                p_to->set_time_index( p_from->get_time_index() );
                p_to->set_frequency_interval( p_from->get_frequency_interval() );
                p_to->set_frequency_index( p_from->get_frequency_index() );
                p_window->set_size( p_from->get_size() );
                return;
            }
    };

    template< class x_type >
    template< template <class> class x_data_type >
    class _window_transformer< x_type >::_data< x_data_type< real_t > >
    {
        public:
            inline void window( const x_data_type< real_t >* p_from, x_data_type< real_t >* p_to, window* p_window )
            {
                register real_t t_norm = 1. / (p_window->sum() * p_window->sum());
                for( index_t t_index = 0; t_index < p_from->get_size(); t_index++ )
                {
                    p_to->at( t_index ) = p_from->at( t_index ) * p_window->at( t_index ) * t_norm;
                }
                return;
            }
    };

    template< class x_type >
    template< template <class> class x_data_type >
    class _window_transformer< x_type >::_data< x_data_type< complex_t > >
    {
        public:
            inline void window( const x_data_type< complex_t >* p_from, x_data_type< complex_t >* p_to, window* p_window )
            {
                register real_t t_norm = 1. / (p_window->sum() * p_window->sum());
                for( index_t t_index = 0; t_index < p_from->get_size(); t_index++ )
                {
                    p_to->at( t_index )[ 0 ] = p_from->at( t_index )[ 0 ] * p_window->at( t_index ) * t_norm;
                    p_to->at( t_index )[ 1 ] = p_from->at( t_index )[ 1 ] * p_window->at( t_index ) * t_norm;
                }
                return;
            }
    };

}

#endif
