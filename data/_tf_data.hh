#ifndef _midge__tf_data_hh_
#define _midge__tf_data_hh_

#include "types.hh"
#include "fourier.hh"
#include "binary.hh"
#include "ascii.hh"
#include "_stream.hh"

namespace midge
{

    template< class x_type >
    class _tf_data :
        public _stream< _tf_data< x_type > >
    {
        public:
            _tf_data();
            virtual ~_tf_data();

        public:
            x_type* raw() const;

            void set_size( const count_t& p_size );
            const count_t& get_size() const;

            void set_time_interval( const real_t& p_time_interval );
            const real_t& get_time_interval() const;

            void set_time_index( const count_t& p_time_index );
            const count_t& get_time_index() const;

            void set_frequency_interval( const real_t& p_frequency_interval );
            const real_t& get_frequency_interval() const;

            void set_frequency_index( const count_t& p_frequency_index );
            const count_t& get_frequency_index() const;

        protected:
            x_type* f_raw;
            count_t f_size;
            real_t f_time_interval;
            count_t f_time_index;
            real_t f_frequency_interval;
            count_t f_frequency_index;

        public:
            command_t command();
            void command( command_t );
            _tf_data& operator>>( _tf_data& p_data );
            _tf_data& operator<<( const _tf_data& p_data );
    };

    template< class x_type >
    _tf_data< x_type >::_tf_data() :
            f_raw( NULL ),
            f_size( 0 ),
            f_time_interval( 1. ),
            f_time_index( 0 ),
            f_frequency_interval( 1. ),
            f_frequency_index( 0 )
    {
    }
    template< class x_type >
    _tf_data< x_type >::~_tf_data()
    {
    }

    template< class x_type >
    x_type* _tf_data< x_type >::raw() const
    {
        return f_raw;
    }

    template< class x_type >
    void _tf_data< x_type >::set_size( const count_t& p_size )
    {
        if( f_size == p_size )
        {
            return;
        }
        f_size = p_size;

        if( f_raw != NULL )
        {
            fourier::get_instance()->free< x_type >( f_raw );
        }
        f_raw = fourier::get_instance()->allocate< x_type >( f_size );

        return;
    }
    template< class x_type >
    const count_t& _tf_data< x_type >::get_size() const
    {
        return f_size;
    }

    template< class x_type >
    void _tf_data< x_type >::set_time_interval( const real_t& p_time_interval )
    {
        f_time_interval = p_time_interval;
    }
    template< class x_type >
    const real_t& _tf_data< x_type >::get_time_interval() const
    {
        return f_time_interval;
    }

    template< class x_type >
    void _tf_data< x_type >::set_time_index( const count_t& p_time_index )
    {
        f_time_index = p_time_index;
    }
    template< class x_type >
    const count_t& _tf_data< x_type >::get_time_index() const
    {
        return f_time_index;
    }

    template< class x_type >
    void _tf_data< x_type >::set_frequency_interval( const real_t& p_frequency_interval )
    {
        f_frequency_interval = p_frequency_interval;
    }
    template< class x_type >
    const real_t& _tf_data< x_type >::get_frequency_interval() const
    {
        return f_frequency_interval;
    }

    template< class x_type >
    void _tf_data< x_type >::set_frequency_index( const count_t& p_frequency_index )
    {
        f_frequency_index = p_frequency_index;
    }
    template< class x_type >
    const count_t& _tf_data< x_type >::get_frequency_index() const
    {
        return f_frequency_index;
    }

    template< class x_type >
    command_t _tf_data< x_type >::command()
    {
        return stream::s_none;
    }
    template< class x_type >
    void _tf_data< x_type >::command( command_t )
    {
        return;
    }

    template< class x_type >
    _tf_data< x_type >& _tf_data< x_type >::operator>>( _tf_data< x_type >& p_data )
    {

        p_data.f_raw = f_raw;
        p_data.f_size = f_size;
        p_data.f_time_interval = f_time_interval;
        p_data.f_time_index = f_time_index;
        p_data.f_frequency_interval = f_frequency_interval;
        p_data.f_frequency_index = f_frequency_index;
        return *this;
    }
    template< class x_type >
    _tf_data< x_type >& _tf_data< x_type >::operator<<( const _tf_data< x_type >& p_data )
    {
        f_raw = p_data.f_raw;
        f_size = p_data.f_size;
        f_time_interval = p_data.f_time_interval;
        f_time_index = p_data.f_time_index;
        f_frequency_interval = p_data.f_frequency_interval;
        f_frequency_index = p_data.f_frequency_index;
        return *this;
    }

    template< class x_data >
    class ascii::pull< _tf_data< x_data > >
    {
        public:
            pull( ascii& p_stream, _tf_data< x_data >& p_data )
            {
                string_t t_hash;
                real_t t_frequency;

                count_t t_size;
                real_t t_time_interval;
                count_t t_time_index;
                real_t t_frequency_interval;
                count_t t_frequency_index;;

                p_stream >> t_hash >> t_size;
                p_stream >> t_hash >> t_time_interval;
                p_stream >> t_hash >> t_time_index;
                p_stream >> t_hash >> t_frequency_interval;
                p_stream >> t_hash >> t_frequency_index;

                p_data.set_size( t_size );
                p_data.set_time_interval( t_time_interval );
                p_data.set_time_index( t_time_index );
                p_data.set_frequency_interval( t_frequency_interval );
                p_data.set_frequency_index( t_frequency_index );
                for( count_t t_index = 0; t_index < t_size; t_index++ )
                {
                    p_stream >> t_frequency >> p_data.raw()[ t_index ];
                }
            }
    };
    template< class x_data >
    class ascii::push< _tf_data< x_data > >
    {
        public:
            push( ascii& p_stream, const _tf_data< x_data >& p_data )
            {
                p_stream << "# " << p_data.get_size() << "\n";
                p_stream << "# " << p_data.get_time_interval() << "\n";
                p_stream << "# " << p_data.get_time_index() << "\n";
                p_stream << "# " << p_data.get_frequency_interval() << "\n";
                p_stream << "# " << p_data.get_frequency_index() << "\n";
                for( count_t t_index = 0; t_index < p_data.get_size(); t_index++ )
                {
                    p_stream << p_data.get_time_index() * p_data.get_time_interval() << " " << (t_index + p_data.get_frequency_index()) * p_data.get_frequency_interval() << " " << p_data.raw()[ t_index ] << "\n";
                }
                p_stream << "\n";
            }
    };

    template< class x_data >
    class binary::pull< _tf_data< x_data > >
    {
        public:
            pull( binary& p_stream, _tf_data< x_data >& p_data )
            {
                count_t t_size;
                real_t t_time_interval;
                count_t t_time_index;
                real_t t_frequency_interval;
                count_t t_frequency_index;

                p_stream >> t_size;
                p_stream >> t_time_interval;
                p_stream >> t_time_index;
                p_stream >> t_frequency_interval;
                p_stream >> t_frequency_index;

                p_data.set_size( t_size );
                p_data.set_time_interval( t_time_interval );
                p_data.set_time_index( t_time_index );
                p_data.set_frequency_interval( t_frequency_interval );
                p_data.set_frequency_index( t_frequency_index );
                p_stream.f_fstream.read( reinterpret_cast< char* >( p_data.raw() ), t_size * sizeof(x_data) );
            }
    };
    template< class x_data >
    class binary::push< _tf_data< x_data > >
    {
        public:
            push( binary& p_stream, const _tf_data< x_data >& p_data )
            {
                p_stream << p_data.get_size();
                p_stream << p_data.get_time_interval();
                p_stream << p_data.get_time_index();
                p_stream << p_data.get_frequency_interval();
                p_stream << p_data.get_frequency_index();
                p_stream.f_fstream.write( reinterpret_cast< const char* >( p_data.raw() ), p_data.get_size() * sizeof(x_data) );
            }
    };

}

#endif
