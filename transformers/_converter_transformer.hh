#ifndef _midge__converter_transformer_hh_
#define _midge__converter_transformer_hh_

#include "transformer.hh"
#include "_t_data.hh"
#include "_f_data.hh"
#include "_tf_data.hh"
#include "types.hh"
#include "typelist.hh"
#include "macros.hh"

#include <cmath>

namespace midge
{

    template< class x_in_type, class x_out_type >
    class _converter_transformer;

    template< template< class > class x_type, class x_in_type, class x_out_type >
    class _converter_transformer< x_type< x_in_type >, x_type< x_out_type > > :
        public _transformer< _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >, typelist_1( x_type< x_in_type > ),typelist_1( x_type< x_out_type > ) >
    {
        public:
            typedef _transformer< _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >, typelist_1( x_type< x_in_type > ),typelist_1( x_type< x_out_type > ) > parent;

        public:
            _converter_transformer();
            virtual ~_converter_transformer();

        public:
            accessible( string_t, mode )
            accessible( count_t, length )

        protected:
            void initialize();
            void execute();
            void finalize();

        private:
            template< class x_header_in_type, class x_header_out_type >
            class _header;

            template< class x_data_in_type, class x_data_out_type >
            class _data;
    };

    template< template< class > class x_type, class x_in_type, class x_out_type >
    _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::_converter_transformer() :
            f_mode( "" ),
            f_length( 10 )
    {
    }
    template< template< class > class x_type, class x_in_type, class x_out_type >
    _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::~_converter_transformer()
    {
    }

    template< template< class > class x_type, class x_in_type, class x_out_type >
    void _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::initialize()
    {
        parent::template out_buffer< 0 >().initialize( f_length );
        return;
    }

    template< template< class > class x_type, class x_in_type, class x_out_type >
    void _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::execute()
    {
        command_t t_in_command;
        x_type< x_in_type > t_in_data;
        x_type< x_out_type > t_out_data;

        void (*t_header)( const x_type< x_in_type >&, x_type< x_out_type >& ) = _header< x_type< x_in_type >, x_type< x_out_type > >::function();
        void (*t_data)( const x_type< x_in_type >&, x_type< x_out_type >& ) = _data< x_type< x_in_type >, x_type< x_out_type > >::function( f_mode );

        while( true )
        {
            parent::template in_stream< 0 >() >> t_in_data;
            parent::template out_stream< 0 >() >> t_out_data;
            t_in_command = parent::template in_stream< 0 >().command();

            if( t_in_command == stream::s_start )
            {
                t_header( t_in_data, t_out_data );

                parent::template out_stream< 0 >().command( stream::s_start );
                parent::template out_stream< 0 >() << t_out_data;
                parent::template in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_run )
            {
                t_header( t_in_data, t_out_data );
                t_data( t_in_data, t_out_data );

                parent::template out_stream< 0 >().command( stream::s_run );
                parent::template out_stream< 0 >() << t_out_data;
                parent::template in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_stop )
            {
                t_header( t_in_data, t_out_data );

                parent::template out_stream< 0 >().command( stream::s_stop );
                parent::template out_stream< 0 >() << t_out_data;
                parent::template in_stream< 0 >() << t_in_data;
                continue;
            }
            if( t_in_command == stream::s_exit )
            {
                t_header( t_in_data, t_out_data );

                parent::template out_stream< 0 >().command( stream::s_exit );
                parent::template out_stream< 0 >() << t_out_data;
                parent::template in_stream< 0 >() << t_in_data;
                return;
            }
        }

        return;
    }

    template< template< class > class x_type, class x_in_type, class x_out_type >
    void _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::finalize()
    {
        parent::template out_buffer< 0 >().finalize();
        return;
    }


    template< template< class > class x_type, class x_in_type, class x_out_type >
    template< class x_header_in_type, class x_header_out_type >
    class _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::_header< _t_data< x_header_in_type >, _t_data< x_header_out_type > >
    {
        public:
            static void (*function())( const _t_data< x_header_in_type >&, _t_data< x_header_out_type >& )
            {
                return &copy;
            }

            static void copy( const _t_data< x_header_in_type >& p_from, _t_data< x_header_out_type >& p_to )
            {
                p_to.set_size( p_from.get_size() );
                p_to.set_time_interval( p_from.get_time_interval() );
                p_to.set_time_index( p_from.get_time_index() );
                return;
            }
    };

    template< template< class > class x_type, class x_in_type, class x_out_type >
    template< class x_header_in_type, class x_header_out_type >
    class _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::_header< _f_data< x_header_in_type >, _f_data< x_header_out_type > >
    {
        public:
            static void (*function())( const _f_data< x_header_in_type >&, _f_data< x_header_out_type >& )
            {
                return &copy;
            }

            static void copy( const _f_data< x_header_in_type >& p_from, _f_data< x_header_out_type >& p_to )
            {
                p_to.set_size( p_from.get_size() );
                p_to.set_frequency_interval( p_from.get_frequency_interval() );
                p_to.set_frequency_index( p_from.get_frequency_index() );
                return;
            }
    };

    template< template< class > class x_type, class x_in_type, class x_out_type >
    template< class x_header_in_type, class x_header_out_type >
    class _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::_header< _tf_data< x_header_in_type >, _tf_data< x_header_out_type > >
    {
        public:
            static void (*function())( const _tf_data< x_header_in_type >&, _tf_data< x_header_out_type >& )
            {
                return &copy;
            }

            static void copy( const _tf_data< x_header_in_type >& p_from, _tf_data< x_header_out_type >& p_to )
            {
                p_to.set_size( p_from.get_size() );
                p_to.set_time_interval( p_from.get_time_interval() );
                p_to.set_time_index( p_from.get_time_index() );
                p_to.set_frequency_interval( p_from.get_frequency_interval() );
                p_to.set_frequency_index( p_from.get_frequency_index() );
                return;
            }
    };

    template< template< class > class x_type, class x_in_type, class x_out_type >
    template< template< class > class x_data_type >
    class _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::_data< x_data_type< real_t >, x_data_type< complex_t > >
    {
        public:
            static void (*function( const string& p_string ))( const x_data_type< real_t >&, x_data_type< complex_t >& )
            {
                if( p_string == string( "real" ) )
                {
                    return &real;
                }
                if( p_string == string( "imaginary" ) )
                {
                    return &imaginary;
                }
                return NULL;
            }

            static inline void real( const x_data_type< real_t >& p_from, x_data_type< complex_t >& p_to )
            {
                for( count_t t_index = 0; t_index < p_from.get_size(); t_index++ )
                {
                    p_to.raw()[ t_index ][ 0 ] = p_from.raw()[ t_index ];
                    p_to.raw()[ t_index ][ 1 ] = 0.;
                }
                return;
            }

            static inline void imaginary( const x_data_type< real_t >& p_from, x_data_type< complex_t >& p_to )
            {
                for( count_t t_index = 0; t_index < p_from.get_size(); t_index++ )
                {
                    p_to.raw()[ t_index ][ 0 ] = 0.;
                    p_to.raw()[ t_index ][ 1 ] = p_from.raw()[ t_index ];
                }
                return;
            }
    };

    template< template< class > class x_type, class x_in_type, class x_out_type >
    template< template< class > class x_data_type >
    class _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::_data< x_data_type< complex_t >, x_data_type< real_t > >
    {
        public:
            static void (*function( const string& p_string ))( const x_data_type< complex_t >&, x_data_type< real_t >& )
            {
                if( p_string == string( "real" ) )
                {
                    return &real;
                }
                if( p_string == string( "imaginary" ) )
                {
                    return &imaginary;
                }
                if( p_string == string( "modulus" ) )
                {
                    return &modulus;
                }
                if( p_string == string( "argument" ) )
                {
                    return &argument;
                }
                return NULL;
            }

            static inline void real( const x_data_type< complex_t >& p_from, x_data_type< real_t >& p_to )
            {
                for( count_t t_index = 0; t_index < p_from.get_size(); t_index++ )
                {
                    p_to.raw()[ t_index ] = p_from.raw()[ t_index ][ 0 ];
                }
                return;
            }

            static inline void imaginary( const x_data_type< complex_t >& p_from, x_data_type< real_t >& p_to )
            {
                for( count_t t_index = 0; t_index < p_from.get_size(); t_index++ )
                {
                    p_to.raw()[ t_index ] = p_from.raw()[ t_index ][ 0 ];
                }
                return;
            }

            static inline void modulus( const x_data_type< complex_t >& p_from, x_data_type< real_t >& p_to )
            {
                for( count_t t_index = 0; t_index < p_from.get_size(); t_index++ )
                {
                    p_to.raw()[ t_index ] = sqrt( p_from.raw()[ t_index ][ 0 ] * p_from.raw()[ t_index ][ 0 ] + p_from.raw()[ t_index ][ 1 ] * p_from.raw()[ t_index ][ 1 ] );
                }
                return;
            }

            static inline void argument( const x_data_type< complex_t >& p_from, x_data_type< real_t >& p_to )
            {
                for( count_t t_index = 0; t_index < p_from.get_size(); t_index++ )
                {
                    p_to.raw()[ t_index ] = atan2( p_from.raw()[ t_index ][ 1 ], p_from.raw()[ t_index ][ 0 ] );
                }
                return;
            }
    };

}

#endif
