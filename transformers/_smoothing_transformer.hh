#ifndef _midge__smoothing_transformer_hh_
#define _midge__smoothing_transformer_hh_

#include "transformer.hh"
#include "_t_data.hh"
#include "_f_data.hh"
#include "_tf_data.hh"
#include "types.hh"
#include "typelist.hh"
#include "macros.hh"
#include "window.hh"

namespace midge
{

    template< class x_type >
    class _smoothing_transformer :
        public _transformer< _smoothing_transformer< x_type >, typelist_1( x_type ),typelist_1( x_type ) >
    {
        public:
            typedef _transformer< _smoothing_transformer< x_type >, typelist_1( x_type ),typelist_1( x_type ) > parent;

        public:
            _smoothing_transformer();
            virtual ~_smoothing_transformer();

        public:
            accessible( count_t, width )
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

            static int s_initializer;
    };

    template< class x_type >
    _smoothing_transformer< x_type >::_smoothing_transformer() :
            f_width( 0 ),
            f_window( NULL ),
            f_length( 10 )
    {
    }
    template< class x_type >
        _smoothing_transformer< x_type >::~_smoothing_transformer()
    {
    }

    template< class x_type >
    void _smoothing_transformer< x_type >::initialize()
    {
        parent::template out_buffer< 0 >().initialize( f_length );

        if( f_window == NULL )
        {
            throw error() << "smoothing transformer window unset";
        }

        return;
    }

    template< class x_type >
    void _smoothing_transformer< x_type >::execute()
    {
        command_t t_in_command;
        x_type t_in_data;
        x_type t_out_data;

        _header< x_type > t_header;
        _data< x_type > t_data;

        while( true )
        {
            parent::template in_stream< 0 >() >> t_in_data;
            parent::template out_stream< 0 >() >> t_out_data;
            t_in_command = parent::template in_stream< 0 >().command();

            if( t_in_command == stream::s_start )
            {
                t_header.copy( t_in_data, t_out_data );
                f_window->set_size( 2 * f_width + 1 );

                parent::template out_stream< 0 >().command( stream::s_start );
                parent::template out_stream< 0 >() << t_out_data;
                parent::template in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_run )
            {
                t_header.copy( t_in_data, t_out_data );
                t_data.smooth( t_in_data, t_out_data, *f_window );

                parent::template out_stream< 0 >().command( stream::s_run );
                parent::template out_stream< 0 >() << t_out_data;
                parent::template in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_stop )
            {
                t_header.copy( t_in_data, t_out_data );

                parent::template out_stream< 0 >().command( stream::s_stop );
                parent::template out_stream< 0 >() << t_out_data;
                parent::template in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_exit )
            {
                t_header.copy( t_in_data, t_out_data );

                parent::template out_stream< 0 >().command( stream::s_exit );
                parent::template out_stream< 0 >() << t_out_data;
                parent::template in_stream< 0 >() << t_in_data;
                return;
            }
        }

        return;
    }

    template< class x_type >
    void _smoothing_transformer< x_type >::finalize()
    {
        parent::template out_buffer< 0 >().finalize();

        delete f_window;

        return;
    }


    template< class x_type >
    template< class x_header_type >
    class _smoothing_transformer< x_type >::_header< _t_data< x_header_type > >
    {
        public:
            inline void copy( const _t_data< x_header_type >& p_from, _t_data< x_header_type >& p_to )
            {
                p_to.set_size( p_from.get_size() );
                p_to.set_time_interval( p_from.get_time_interval() );
                p_to.set_time_index( p_from.get_time_index() );
                return;
            }
    };

    template< class x_type >
    template< class x_header_type >
    class _smoothing_transformer< x_type >::_header< _f_data< x_header_type > >
    {
        public:
            inline void copy( const _f_data< x_header_type >& p_from, _f_data< x_header_type >& p_to )
            {
                p_to.set_size( p_from.get_size() );
                p_to.set_frequency_interval( p_from.get_frequency_interval() );
                p_to.set_frequency_index( p_from.get_frequency_index() );
                return;
            }
    };

    template< class x_type >
    template< class x_header_type >
    class _smoothing_transformer< x_type >::_header< _tf_data< x_header_type > >
    {
        public:
            inline void copy( const _tf_data< x_header_type >& p_from, _tf_data< x_header_type >& p_to )
            {
                p_to.set_size( p_from.get_size() );
                p_to.set_time_interval( p_from.get_time_interval() );
                p_to.set_time_index( p_from.get_time_index() );
                p_to.set_frequency_interval( p_from.get_frequency_interval() );
                p_to.set_frequency_index( p_from.get_frequency_index() );
                return;
            }
    };

    template< class x_type >
    template< template <class> class x_data_type >
    class _smoothing_transformer< x_type >::_data< x_data_type< real_t > >
    {
        public:
            inline void smooth( const x_data_type< real_t >& p_from, x_data_type< real_t >& p_to, window& p_window )
            {
                register real_t t_value;
                register real_t t_norm;
                register count_t t_size = p_from.get_size();
                register count_t t_width = (p_window.get_size() - 1) / 2;
                for( count_t t_index = 0; t_index < t_size; t_index++ )
                {
                    t_value = 0.;
                    t_norm = 0.;
                    for( count_t t_sub = 0; t_sub < 2 * t_width + 1; t_sub++ )
                    {
                        if( t_index + t_sub < t_width )
                        {
                            continue;
                        }
                        if( t_index + t_sub > t_width + t_size - 1 )
                        {
                            continue;
                        }
                        t_value += p_from.raw()[ t_index + t_sub - t_width ] * p_window.raw()[ t_sub ];
                        t_norm += p_window.raw()[ t_sub ];
                    }
                    p_to.raw()[ t_index ] = t_value / t_norm;
                }
                return;
            }
    };

    template< class x_type >
    template< template <class> class x_data_type >
    class _smoothing_transformer< x_type >::_data< x_data_type< complex_t > >
    {
        public:
            inline void smooth( const x_data_type< complex_t >& p_from, x_data_type< complex_t >& p_to, window& p_window )
            {
                register complex_t t_value;
                register real_t t_norm;
                register count_t t_size = p_from.get_size();
                register count_t t_width = (p_window.get_size() - 1) / 2;
                for( count_t t_index = 0; t_index < t_size; t_index++ )
                {
                    t_value[ 0 ] = 0.;
                    t_value[ 1 ] = 0.;
                    t_norm = 0.;
                    for( count_t t_sub = 0; t_sub < 2 * t_width + 1; t_sub++ )
                    {
                        if( t_index + t_sub < t_width )
                        {
                            continue;
                        }
                        if( t_index + t_sub > t_width + t_size - 1 )
                        {
                            continue;
                        }
                        t_value[ 0 ] += p_from.raw()[ t_index + t_sub - t_width ][ 0 ] * p_window.raw()[ t_sub ];
                        t_value[ 1 ] += p_from.raw()[ t_index + t_sub - t_width ][ 1 ] * p_window.raw()[ t_sub ];
                        t_norm += p_window.raw()[ t_sub ];
                    }
                    p_to.raw()[ t_index ][ 0 ] = t_value[ 0 ] / t_norm;
                    p_to.raw()[ t_index ][ 1 ] = t_value[ 1 ] / t_norm;
                }
                return;
            }
    };

}

#endif