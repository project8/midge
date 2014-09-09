#ifndef _midge__difference_transformer_hh_
#define _midge__difference_transformer_hh_

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

    template< class x_type >
    class _difference_transformer :
        public _transformer< _difference_transformer< x_type >, typelist_2( x_type, x_type ),typelist_1( x_type ) >
    {
        public:
            typedef _transformer< _difference_transformer< x_type >, typelist_2( x_type, x_type ),typelist_1( x_type ) > parent;

        public:
            _difference_transformer();
            virtual ~_difference_transformer();

        public:
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
    _difference_transformer< x_type >::_difference_transformer() :
            f_length( 10 )
    {
    }
    template< class x_type >
        _difference_transformer< x_type >::~_difference_transformer()
    {
    }

    template< class x_type >
    void _difference_transformer< x_type >::initialize()
    {
        parent::template out_buffer< 0 >().initialize( f_length );
        return;
    }

    template< class x_type >
    void _difference_transformer< x_type >::execute()
    {
        enum_t t_in_zero_command;
        const x_type* t_in_zero_data;
        enum_t t_in_one_command;
        const x_type* t_in_one_data;
        x_type* t_out_data;

        _header< x_type > t_header;
        _data< x_type > t_data;

        while( true )
        {
            t_in_zero_command = parent::template in_stream< 0 >().get();
            t_in_zero_data = parent::template in_stream< 0 >().data();

            t_in_one_command = parent::template in_stream< 1 >().get();
            t_in_one_data = parent::template in_stream< 1 >().data();

            t_out_data = parent::template out_stream< 0 >().data();

            if( (t_in_zero_command == stream::s_start) && (t_in_one_command == stream::s_start) )
            {
                t_header.copy( t_in_zero_data, t_in_one_data, t_out_data );

                parent::template out_stream< 0 >().set( stream::s_start );
                continue;
            }
            if( (t_in_zero_command == stream::s_run) && (t_in_one_command == stream::s_run) )
            {
                t_header.copy( t_in_zero_data, t_in_one_data, t_out_data );
                t_data.difference( t_in_zero_data, t_in_one_data, t_out_data );

                parent::template out_stream< 0 >().set( stream::s_run );
                continue;
            }
            if( (t_in_zero_command == stream::s_stop) || (t_in_one_command == stream::s_stop) )
            {
                t_header.copy( t_in_zero_data, t_in_one_data, t_out_data );

                parent::template out_stream< 0 >().set( stream::s_stop );
                continue;
            }
            if( (t_in_zero_command == stream::s_exit) || (t_in_one_command == stream::s_exit) )
            {
                t_header.copy( t_in_zero_data, t_in_one_data, t_out_data );

                parent::template out_stream< 0 >().set( stream::s_exit );
                return;
            }
        }

        return;
    }

    template< class x_type >
    void _difference_transformer< x_type >::finalize()
    {
        parent::template out_buffer< 0 >().finalize();
        return;
    }


    template< class x_type >
    template< class x_header_type >
    class _difference_transformer< x_type >::_header< _t_data< x_header_type > >
    {
        public:
            inline void copy( const _t_data< x_header_type >* p_from_zero, const _t_data< x_header_type >* p_from_one, _t_data< x_header_type >* p_to )
            {
                if( p_from_zero->get_size() != p_from_one->get_size() )
                {
                    throw error() << "difference transformer size mismatch";
                }

                if( p_from_zero->get_time_interval() != p_from_one->get_time_interval() )
                {
                    throw error() << "difference transformer time interval mismatch";
                }

                if( p_from_zero->get_time_index() != p_from_one->get_time_index() )
                {
                    throw error() << "difference transformer time index mismatch";
                }

                p_to->set_size( p_from_zero->get_size() );
                p_to->set_time_interval( p_from_zero->get_time_interval() );
                p_to->set_time_index( p_from_zero->get_time_index() );
                return;
            }
    };

    template< class x_type >
    template< class x_header_type >
    class _difference_transformer< x_type >::_header< _f_data< x_header_type > >
    {
        public:
            inline void copy( const _f_data< x_header_type >* p_from_zero, const _f_data< x_header_type >* p_from_one, _f_data< x_header_type >* p_to )
            {
                if( p_from_zero->get_size() != p_from_one->get_size() )
                {
                    throw error() << "difference transformer size mismatch";
                }

                if( p_from_zero->get_frequency_interval() != p_from_one->get_frequency_interval() )
                {
                    throw error() << "difference transformer frequency interval mismatch";
                }

                if( p_from_zero->get_frequency_index() != p_from_one->get_frequency_index() )
                {
                    throw error() << "difference transformer frequency index mismatch";
                }

                p_to->set_size( p_from_zero->get_size() );
                p_to->set_frequency_interval( p_from_zero->get_frequency_interval() );
                p_to->set_frequency_index( p_from_zero->get_frequency_index() );
                return;
            }
    };

    template< class x_type >
    template< class x_header_type >
    class _difference_transformer< x_type >::_header< _tf_data< x_header_type > >
    {
        public:
            inline void copy( const _tf_data< x_header_type >* p_from_zero, const _tf_data< x_header_type >* p_from_one, _tf_data< x_header_type >* p_to )
            {
                if( p_from_zero->get_size() != p_from_one->get_size() )
                {
                    throw error() << "difference transformer size mismatch";
                }

                if( p_from_zero->get_time_interval() != p_from_one->get_time_interval() )
                {
                    throw error() << "difference transformer time interval mismatch";
                }

                if( p_from_zero->get_time_index() != p_from_one->get_time_index() )
                {
                    throw error() << "difference transformer time index mismatch";
                }

                if( p_from_zero->get_frequency_interval() != p_from_one->get_frequency_interval() )
                {
                    throw error() << "difference transformer frequency interval mismatch";
                }

                if( p_from_zero->get_frequency_index() != p_from_one->get_frequency_index() )
                {
                    throw error() << "difference transformer frequency index mismatch";
                }

                p_to->set_size( p_from_zero->get_size() );
                p_to->set_time_interval( p_from_zero->get_time_interval() );
                p_to->set_time_index( p_from_zero->get_time_index() );
                p_to->set_frequency_interval( p_from_zero->get_frequency_interval() );
                p_to->set_frequency_index( p_from_zero->get_frequency_index() );
                return;
            }
    };

    template< class x_type >
    template< template <class> class x_data_type >
    class _difference_transformer< x_type >::_data< x_data_type< real_t > >
    {
        public:
            inline void difference( const x_data_type< real_t >* p_from_zero, const x_data_type< real_t >* p_from_one, x_data_type< real_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_from_zero->get_size(); t_index++ )
                {
                    p_to->at( t_index ) = p_from_zero->at( t_index ) - p_from_one->at( t_index );
                }
                return;
            }
    };

    template< class x_type >
    template< template <class> class x_data_type >
    class _difference_transformer< x_type >::_data< x_data_type< complex_t > >
    {
        public:
            inline void difference( const x_data_type< complex_t >* p_from_zero, const x_data_type< complex_t >* p_from_one, x_data_type< complex_t >* p_to )
            {
                for( index_t t_index = 0; t_index < p_from_zero->get_size(); t_index++ )
                {
                    p_to->at( t_index )[ 0 ] = p_from_zero->at( t_index )[ 0 ] - p_from_one->at( t_index )[ 0 ];
                    p_to->at( t_index )[ 1 ] = p_from_zero->at( t_index )[ 1 ] - p_from_one->at( t_index )[ 1 ];
                }
                return;
            }
    };

}

#endif
