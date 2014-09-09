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
        enum_t t_command;
        const x_type< x_in_type >* t_in_data;
        x_type< x_out_type >* t_out_data;

        _header< x_type< x_in_type >, x_type< x_out_type > > t_header;
        _data< x_type< x_in_type >, x_type< x_out_type > > t_data( f_mode );

        while( true )
        {
            t_command = parent::template in_stream< 0 >().get();
            t_in_data = parent::template in_stream< 0 >().data();
            t_out_data = parent::template out_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                t_header.copy( t_in_data, t_out_data );

                parent::template out_stream< 0 >().set( stream::s_start );
                continue;
            }
            if( t_command == stream::s_run )
            {
                t_header.copy( t_in_data, t_out_data );
                t_data.convert( t_in_data, t_out_data );

                parent::template out_stream< 0 >().set( stream::s_run );
                continue;
            }
            if( t_command == stream::s_stop )
            {
                t_header.copy( t_in_data, t_out_data );

                parent::template out_stream< 0 >().set( stream::s_stop );
                continue;
            }
            if( t_command == stream::s_exit )
            {
                t_header.copy( t_in_data, t_out_data );

                parent::template out_stream< 0 >().set( stream::s_exit );
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
            inline void copy( const _t_data< x_header_in_type >* p_from, _t_data< x_header_out_type >* p_to )
            {
                p_to->set_size( p_from->get_size() );
                p_to->set_time_interval( p_from->get_time_interval() );
                p_to->set_time_index( p_from->get_time_index() );
                return;
            }
    };

    template< template< class > class x_type, class x_in_type, class x_out_type >
    template< class x_header_in_type, class x_header_out_type >
    class _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::_header< _f_data< x_header_in_type >, _f_data< x_header_out_type > >
    {
        public:
            inline void copy( const _f_data< x_header_in_type >* p_from, _f_data< x_header_out_type >* p_to )
            {
                p_to->set_size( p_from->get_size() );
                p_to->set_frequency_interval( p_from->get_frequency_interval() );
                p_to->set_frequency_index( p_from->get_frequency_index() );
                return;
            }
    };

    template< template< class > class x_type, class x_in_type, class x_out_type >
    template< class x_header_in_type, class x_header_out_type >
    class _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::_header< _tf_data< x_header_in_type >, _tf_data< x_header_out_type > >
    {
        public:
            inline void copy( const _tf_data< x_header_in_type >* p_from, _tf_data< x_header_out_type >* p_to )
            {
                p_to->set_size( p_from->get_size() );
                p_to->set_time_interval( p_from->get_time_interval() );
                p_to->set_time_index( p_from->get_time_index() );
                p_to->set_frequency_interval( p_from->get_frequency_interval() );
                p_to->set_frequency_index( p_from->get_frequency_index() );
                return;
            }
    };

    template< template< class > class x_type, class x_in_type, class x_out_type >
    template< template< class > class x_data_type >
    class _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::_data< x_data_type< real_t >, x_data_type< complex_t > >
    {
        public:
            _data( const string& p_string )
            {
                if( p_string == string( "real" ) )
                {
                    f_member = &_data::real;
                }
                if( p_string == string( "imaginary" ) )
                {
                    f_member = &_data::imaginary;
                }
            }

            inline void convert( const x_data_type< real_t >* p_from, x_data_type< complex_t >* p_to )
            {
                (this->*f_member)( p_from, p_to );
                return;
            }

            inline void real( const x_data_type< real_t >* p_from, x_data_type< complex_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_from->get_size(); t_index++ )
                {
                    p_to->at( t_index )[ 0 ] = p_from->at( t_index );
                    p_to->at( t_index )[ 1 ] = 0.;
                }
                return;
            }

            inline void imaginary( const x_data_type< real_t >* p_from, x_data_type< complex_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_from->get_size(); t_index++ )
                {
                    p_to->at( t_index )[ 0 ] = 0.;
                    p_to->at( t_index )[ 1 ] = p_from->at( t_index );
                }
                return;
            }

        private:
            void (_data::*f_member)( const x_data_type< real_t >*, x_data_type< complex_t >* );
    };

    template< template< class > class x_type, class x_in_type, class x_out_type >
    template< template< class > class x_data_type >
    class _converter_transformer< x_type< x_in_type >, x_type< x_out_type > >::_data< x_data_type< complex_t >, x_data_type< real_t > >
    {
        public:
            _data( const string& p_string )
            {
                if( p_string == string( "real" ) )
                {
                    f_member = &_data::real;
                }
                if( p_string == string( "imaginary" ) )
                {
                    f_member = &_data::imaginary;
                }
                if( p_string == string( "modulus" ) )
                {
                    f_member = &_data::modulus;
                }
                if( p_string == string( "argument" ) )
                {
                    f_member = &_data::argument;
                }
            }

            inline void convert( const x_data_type< complex_t >* p_from, x_data_type< real_t >* p_to )
            {
                (this->*f_member)( p_from, p_to );
                return;
            }

            inline void real( const x_data_type< complex_t >* p_from, x_data_type< real_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_from->get_size(); t_index++ )
                {
                    p_to->at( t_index ) = p_from->at( t_index )[ 0 ];
                }
                return;
            }

            inline void imaginary( const x_data_type< complex_t >* p_from, x_data_type< real_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_from->get_size(); t_index++ )
                {
                    p_to->at( t_index ) = p_from->at( t_index )[ 1 ];
                }
                return;
            }

            inline void modulus( const x_data_type< complex_t >* p_from, x_data_type< real_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_from->get_size(); t_index++ )
                {
                    p_to->at( t_index ) = sqrt( p_from->at( t_index )[ 0 ] * p_from->at( t_index )[ 0 ] + p_from->at( t_index )[ 1 ] * p_from->at( t_index )[ 1 ] );
                }
                return;
            }

            inline void argument( const x_data_type< complex_t >* p_from, x_data_type< real_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_from->get_size(); t_index++ )
                {
                    p_to->at( t_index ) = atan2( p_from->at( t_index )[ 1 ], p_from->at( t_index )[ 0 ] );
                }
                return;
            }

        private:
            void (_data::*f_member)( const x_data_type< complex_t >*, x_data_type< real_t >* );
    };

}

#endif
