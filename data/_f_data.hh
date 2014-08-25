#ifndef _midge__f_data_hh_
#define _midge__f_data_hh_

#include "types.hh"
#include "fourier.hh"
#include "binary.hh"
#include "ascii.hh"
#include "_stream.hh"

namespace midge
{

    template< class x_type >
    class _f_data :
        public _stream< _f_data< x_type > >
    {
        public:
            _f_data() :
                    f_raw( NULL ),
                    f_size( 0 ),
                    f_frequency_interval( 1. ),
                    f_frequency_index( 0 )
            {
            }
            virtual ~_f_data()
            {
                if( f_raw != NULL )
                {
                    fourier::get_instance()->free< x_type >( f_raw );
                }
            }

        public:
            x_type* raw()
            {
                return f_raw;
            }
            x_type* raw() const
            {
                return f_raw;
            }

            void set_size( const count_t& p_size )
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
            const count_t& get_size() const
            {
                return f_size;
            }

            void set_frequency_interval( const real_t& p_frequency_interval )
            {
                f_frequency_interval = p_frequency_interval;
                return;
            }
            const real_t& get_frequency_interval() const
            {
                return f_frequency_interval;
            }

            void set_frequency_index( const count_t& p_frequency_index )
            {
                f_frequency_index = p_frequency_index;
                return;
            }
            const count_t& get_frequency_index() const
            {
                return f_frequency_index;
            }

        protected:
            x_type* f_raw;
            count_t f_size;
            real_t f_frequency_interval;
            count_t f_frequency_index;

        public:
            command_t command()
            {
                return stream::s_none;
            }
            void command( command_t )
            {
                return;
            }

            _f_data& operator>>( _f_data& p_data )
            {
                p_data.f_raw = f_raw;
                p_data.f_size = f_size;
                p_data.f_frequency_interval = f_frequency_interval;
                p_data.f_frequency_index = f_frequency_index;
                return *this;
            }
            _f_data& operator<<( const _f_data& p_data )
            {
                f_raw = p_data.f_raw;
                f_size = p_data.f_size;
                f_frequency_interval = p_data.f_frequency_interval;
                f_frequency_index = p_data.f_frequency_index;
                return *this;
            }
    };

    template< class x_data >
    class ascii::pull< _f_data< x_data > >
    {
        public:
            pull( ascii& p_stream, _f_data< x_data >& p_data )
            {
                string_t t_hash;
                real_t t_frequency;

                count_t t_size;
                real_t t_frequency_interval;
                count_t t_frequency_index;

                p_stream >> t_hash >> t_size;
                p_stream >> t_hash >> t_frequency_interval;
                p_stream >> t_hash >> t_frequency_index;

                p_data.set_size( t_size );
                p_data.set_frequency_interval( t_frequency_interval );
                p_data.set_frequency_index( t_frequency_index );
                for( count_t t_index = 0; t_index < t_size; t_index++ )
                {
                    p_stream >> t_frequency >> p_data.raw()[ t_index ];
                }
            }
    };
    template< class x_data >
    class ascii::push< _f_data< x_data > >
    {
        public:
            push( ascii& p_stream, const _f_data< x_data >& p_data )
            {
                p_stream << "# " << p_data.get_size() << "\n";
                p_stream << "# " << p_data.get_frequency_interval() << "\n";
                p_stream << "# " << p_data.get_frequency_index() << "\n";
                for( count_t t_index = 0; t_index < p_data.get_size(); t_index++ )
                {
                    p_stream << (t_index + p_data.get_frequency_index()) * p_data.get_frequency_interval() << " " << p_data.raw()[ t_index ] << "\n";
                }
                p_stream << "\n";
            }
    };

    template< class x_data >
    class binary::pull< _f_data< x_data > >
    {
        public:
            pull( binary& p_stream, _f_data< x_data >& p_data )
            {
                count_t t_size;
                real_t t_frequency_interval;
                count_t t_frequency_index;

                p_stream >> t_size;
                p_stream >> t_frequency_interval;
                p_stream >> t_frequency_index;

                p_data.set_size( t_size );
                p_data.set_frequency_interval( t_frequency_interval );
                p_data.set_frequency_index( t_frequency_index );
                for( count_t t_index = 0; t_index < t_size; t_index++ )
                {
                    p_stream >> p_data.raw()[ t_index ];
                }
            }
    };
    template< class x_data >
    class binary::push< _f_data< x_data > >
    {
        public:
            push( binary& p_stream, const _f_data< x_data >& p_data )
            {
                p_stream << p_data.get_size();
                p_stream << p_data.get_frequency_interval();
                p_stream << p_data.get_frequency_index();
                for( count_t t_index = 0; t_index < p_data.get_size(); t_index++ )
                {
                    p_stream << p_data.  raw()[ t_index ];
                }
            }
    };

}

#endif
