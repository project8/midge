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
            composable( window, window )
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
        command_t t_in_command;
        x_type t_in_data;
        x_type t_out_data;

        void (*t_header)( const x_type&, x_type&, window& ) = _header< x_type >::function();
        void (*t_data)( const x_type&, x_type&, window& ) = _data< x_type >::function();

        while( true )
        {
            parent::template in_stream< 0 >() >> t_in_data;
            parent::template out_stream< 0 >() >> t_out_data;
            t_in_command = parent::template in_stream< 0 >().command();

            if( t_in_command == stream::s_start )
            {
                t_header( t_in_data, t_out_data, *f_window );

                parent::template out_stream< 0 >().command( stream::s_start );
                parent::template out_stream< 0 >() << t_out_data;
                parent::template in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_run )
            {
                t_header( t_in_data, t_out_data, *f_window );
                t_data( t_in_data, t_out_data, *f_window );

                parent::template out_stream< 0 >().command( stream::s_run );
                parent::template out_stream< 0 >() << t_out_data;
                parent::template in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_stop )
            {
                t_header( t_in_data, t_out_data, *f_window );

                parent::template out_stream< 0 >().command( stream::s_stop );
                parent::template out_stream< 0 >() << t_out_data;
                parent::template in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_exit )
            {
                t_header( t_in_data, t_out_data, *f_window );

                parent::template out_stream< 0 >().command( stream::s_exit );
                parent::template out_stream< 0 >() << t_out_data;
                parent::template in_stream< 0 >() << t_in_data;
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
            static void (*function())( const _t_data< x_header_type >&, _t_data< x_header_type >&, window& )
            {
                return &copy;
            }

            static void copy( const _t_data< x_header_type >& p_from, _t_data< x_header_type >& p_to, window& p_window )
            {
                p_to.set_size( p_from.get_size() );
                p_to.set_time_interval( p_from.get_time_interval() );
                p_to.set_time_index( p_from.get_time_index() );
                p_window.set_size( p_from.get_size() );
                return;
            }
    };

    template< class x_type >
    template< class x_header_type >
    class _window_transformer< x_type >::_header< _f_data< x_header_type > >
    {
        public:
            static void (*function())( const _f_data< x_header_type >&, _f_data< x_header_type >&, window& )
            {
                return &copy;
            }

            static void copy( const _f_data< x_header_type >& p_from, _f_data< x_header_type >& p_to, window& p_window )
            {
                p_to.set_size( p_from.get_size() );
                p_to.set_frequency_interval( p_from.get_frequency_interval() );
                p_to.set_frequency_index( p_from.get_frequency_index() );
                p_window.set_size( p_from.get_size() );
                return;
            }
    };

    template< class x_type >
    template< class x_header_type >
    class _window_transformer< x_type >::_header< _tf_data< x_header_type > >
    {
        public:
            static void (*function())( const _tf_data< x_header_type >&, _tf_data< x_header_type >&, window& )
            {
                return &copy;
            }

            static void copy( const _tf_data< x_header_type >& p_from, _tf_data< x_header_type >& p_to, window& p_window )
            {
                p_to.set_size( p_from.get_size() );
                p_to.set_time_interval( p_from.get_time_interval() );
                p_to.set_time_index( p_from.get_time_index() );
                p_to.set_frequency_interval( p_from.get_frequency_interval() );
                p_to.set_frequency_index( p_from.get_frequency_index() );
                p_window.set_size( p_from.get_size() );
                return;
            }
    };

    template< class x_type >
    template< template <class> class x_data_type >
    class _window_transformer< x_type >::_data< x_data_type< real_t > >
    {
        public:
            static void (*function())( const x_data_type< real_t >&, x_data_type< real_t >&, window& )
            {
                return &window;
            }

            static inline void window( const x_data_type< real_t >& p_from, x_data_type< real_t >& p_to, window& p_window )
            {
                register real_t t_norm = 1. / (p_window.sum() * p_window.sum());
                for( count_t t_index = 0; t_index < p_from.get_size(); t_index++ )
                {
                    p_to.raw()[ t_index ] = p_from.raw()[ t_index ] * p_window.raw()[ t_index ] * t_norm;
                }
                return;
            }
    };

    template< class x_type >
    template< template <class> class x_data_type >
    class _window_transformer< x_type >::_data< x_data_type< complex_t > >
    {
        public:
            static void (*function())( const x_data_type< complex_t >&, x_data_type< complex_t >&, window& )
            {
                return &window;
            }

            static inline void window( const x_data_type< complex_t >& p_from, x_data_type< complex_t >& p_to, window& p_window )
            {
                register real_t t_norm = 1. / (p_window.sum() * p_window.sum());
                for( count_t t_index = 0; t_index < p_from.get_size(); t_index++ )
                {
                    p_to.raw()[ t_index ][ 0 ] = p_from.raw()[ t_index ][ 0 ] * p_window.raw()[ t_index ] * t_norm;
                    p_to.raw()[ t_index ][ 1 ] = p_from.raw()[ t_index ][ 1 ] * p_window.raw()[ t_index ] * t_norm;
                }
                return;
            }
    };

}

#endif
