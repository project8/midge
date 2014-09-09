#ifndef _midge__average_transformer_hh_
#define _midge__average_transformer_hh_

#include "transformer.hh"
#include "_f_data.hh"
#include "_tf_data.hh"
#include "types.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    template< class x_in_type, class x_out_type >
    class _average_transformer;

    template< template< class > class x_in_type, template< class > class x_out_type, class x_raw_type >
    class _average_transformer< x_in_type< x_raw_type >, x_out_type< x_raw_type > > :
        public _transformer< _average_transformer< x_in_type< x_raw_type >, x_out_type< x_raw_type > >, typelist_1( x_in_type< x_raw_type > ),typelist_1( x_out_type< x_raw_type > ) >
    {
        public:
            typedef _transformer< _average_transformer< x_in_type< x_raw_type >, x_out_type< x_raw_type > >, typelist_1( x_in_type< x_raw_type > ),typelist_1( x_out_type< x_raw_type > ) > parent;

        public:
            _average_transformer();
            virtual ~_average_transformer();

        public:
            ;accessible( count_t, length )

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

    template< template< class > class x_in_type, template< class > class x_out_type, class x_raw_type >
    _average_transformer< x_in_type< x_raw_type >, x_out_type< x_raw_type > >::_average_transformer() :
            f_length( 10 )
    {
    }
    template< template< class > class x_in_type, template< class > class x_out_type, class x_raw_type >
    _average_transformer< x_in_type< x_raw_type >, x_out_type< x_raw_type > >::~_average_transformer()
    {
    }

    template< template< class > class x_in_type, template< class > class x_out_type, class x_raw_type >
    void _average_transformer< x_in_type< x_raw_type >, x_out_type< x_raw_type > >::initialize()
    {
        parent::template out_buffer< 0 >().initialize( f_length );
        return;
    }

    template< template< class > class x_in_type, template< class > class x_out_type, class x_raw_type >
    void _average_transformer< x_in_type< x_raw_type >, x_out_type< x_raw_type > >::execute()
    {
        enum_t t_command;
        const x_in_type< x_raw_type >* t_in_data;
        x_out_type< x_raw_type >* t_out_data;

        _header< x_in_type< x_raw_type >, x_out_type< x_raw_type > > t_header;
        _data< x_in_type< x_raw_type >, x_out_type< x_raw_type > > t_data;

        while( true )
        {
            t_command = parent::template in_stream< 0 >().get();
            t_in_data = parent::template in_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                t_out_data = parent::template out_stream< 0 >().data();
                t_header.copy( t_in_data, t_out_data );
                parent::template out_stream< 0 >().set( stream::s_start );

                t_out_data = parent::template out_stream< 0 >().data();
                t_header.copy( t_in_data, t_out_data );
                t_data.initialize( t_out_data );

                continue;
            }
            if( t_command == stream::s_run )
            {
                t_data.accumulate( t_in_data, t_out_data );

                continue;
            }
            if( t_command == stream::s_stop )
            {
                t_data.finalize( t_out_data );
                parent::template out_stream< 0 >().set( stream::s_run );

                t_out_data = parent::template out_stream< 0 >().data();
                t_header.copy( t_in_data, t_out_data );
                parent::template out_stream< 0 >().set( stream::s_stop );

                continue;
            }
            if( t_command == stream::s_exit )
            {
                parent::template out_stream< 0 >().set( stream::s_exit );
                return;
            }
        }

        return;
    }

    template< template< class > class x_in_type, template< class > class x_out_type, class x_raw_type >
    void _average_transformer< x_in_type< x_raw_type >, x_out_type< x_raw_type > >::finalize()
    {
        parent::template out_buffer< 0 >().finalize();
        return;
    }

    template< template< class > class x_in_type, template< class > class x_out_type, class x_raw_type >
    template< class x_header_raw_type >
    class _average_transformer< x_in_type< x_raw_type >, x_out_type< x_raw_type > >::_header< _tf_data< x_header_raw_type >, _f_data< x_header_raw_type > >
    {
        public:
            inline void copy( const _tf_data< x_header_raw_type >* p_from, _f_data< x_header_raw_type >* p_to )
            {
                p_to->set_size( p_from->get_size() );
                p_to->set_frequency_interval( p_from->get_frequency_interval() );
                p_to->set_frequency_index( p_from->get_frequency_index() );
                return;
            }
    };


    template< template< class > class x_in_type, template< class > class x_out_type, class x_raw_type >
    template< template< class > class x_data_in_type, template< class > class x_data_out_type >
    class _average_transformer< x_in_type< x_raw_type >, x_out_type< x_raw_type > >::_data< x_data_in_type< real_t >, x_data_out_type< real_t > >
    {
        public:
            inline void initialize( x_data_out_type< real_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_to->get_size(); t_index++ )
                {
                    p_to->at( t_index ) = 0.;
                }
                f_count = 0;
                return;
            }

            inline void accumulate( const x_data_in_type< real_t >* p_from, x_data_out_type< real_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_to->get_size(); t_index++ )
                {
                    p_to->at( t_index ) += p_from->at( t_index );
                }
                ++f_count;
                return;
            }

            inline void finalize( x_data_out_type< real_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_to->get_size(); t_index++ )
                {
                    p_to->at( t_index ) /= (real_t)( f_count );
                }
                f_count = 0;
                return;
            }

        private:
            count_t f_count;
    };

    template< template< class > class x_in_type, template< class > class x_out_type, class x_raw_type >
    template< template< class > class x_data_in_type, template< class > class x_data_out_type >
    class _average_transformer< x_in_type< x_raw_type >, x_out_type< x_raw_type > >::_data< x_data_in_type< complex_t >, x_data_out_type< complex_t > >
    {
        public:
            inline void initialize( x_data_out_type< complex_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_to->get_size(); t_index++ )
                {
                    p_to->at( t_index )[ 0 ] = 0.;
                    p_to->at( t_index )[ 1 ] = 0.;
                }
                f_count = 0;
                return;
            }

            inline void accumulate( const x_data_in_type< complex_t >* p_from, x_data_out_type< complex_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_to->get_size(); t_index++ )
                {
                    p_to->at( t_index )[ 0 ] += p_from->at( t_index )[ 0 ];
                    p_to->at( t_index )[ 1 ] += p_from->at( t_index )[ 1 ];
                }
                ++f_count;
                return;
            }

            inline void finalize( x_data_out_type< complex_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_to->get_size(); t_index++ )
                {
                    p_to->at( t_index )[ 0 ] /= (real_t)( f_count );
                    p_to->at( t_index )[ 1 ] /= (real_t)( f_count );
                }
                f_count = 0;
                return;
            }

        private:
            count_t f_count;
    };

}

#endif
