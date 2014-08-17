#ifndef _midge__buffer_hh_
#define _midge__buffer_hh_

#include "_stream.hh"
#include "error.hh"
#include "macros.hh"

namespace midge
{

    template< class x_type >
    class _buffer
    {
        public:
            _buffer() :
                    f_length( 0 ),
                    f_write_state( stream::s_none ),
                    f_write_mutex(),
                    f_write_stream( NULL ),
                    f_read_count( 0 ),
                    f_read_state( NULL ),
                    f_read_data( NULL ),
                    f_read_mutexes( NULL ),
                    f_read_streams( NULL )
            {
            }
            ~_buffer()
            {
                delete f_write_stream;

                for( count_t t_index = 0; t_index < f_length; t_index++ )
                {
                    f_read_state[ t_index ] = stream::s_none;
                }
                delete[] f_read_state;

                for( count_t t_index = 0; t_index < f_length; t_index++ )
                {
                    delete f_read_data[ t_index ];
                }
                delete[] f_read_data;

                for( count_t t_read_index = 0; t_read_index < f_read_count; t_read_index++ )
                {
                    delete[] f_read_mutexes[ t_read_index ];
                    delete f_read_streams[ t_read_index ];
                }
                delete[] f_read_mutexes;
                delete[] f_read_streams;
            }

        public:
        accessible( string_t, name )

        public:
            void initialize( const count_t& p_length )
            {
                f_length = p_length;
                f_read_state = new state_t[ f_length ];
                f_read_data = new x_type*[ f_length ];
                for( count_t t_index = 0; t_index < f_length; t_index++ )
                {
                    f_read_state[ t_index ] = stream::s_none;
                    f_read_data[ t_index ] = new x_type();
                }

                return;
            }

            _stream< x_type >* write()
            {
                if( f_write_stream == NULL )
                {
                    f_write_stream = new _write_stream( *this );
                }
                return f_write_stream;
            }
            _stream< x_type >* read()
            {
                count_t t_new_read_index = f_read_count;
                count_t t_new_read_count = f_read_count + 1;
                mutex** t_new_read_mutexes = new mutex*[ t_new_read_count ];
                _read_stream** t_new_read_streams = new _read_stream*[ t_new_read_count ];

                for( count_t t_index = 0; t_index < f_read_count; t_index++ )
                {
                    t_new_read_mutexes[ t_index ] = f_read_mutexes[ t_index ];
                    t_new_read_streams[ t_index ] = f_read_streams[ t_index ];
                }

                f_read_count = t_new_read_count;

                delete[] f_read_mutexes;
                f_read_mutexes = t_new_read_mutexes;
                f_read_mutexes[ t_new_read_index ] = new mutex[ f_length ];
                f_read_mutexes[ t_new_read_index ][ 0 ].lock();
                f_read_mutexes[ t_new_read_index ][ f_length - 1 ].lock();

                delete[] f_read_streams;
                f_read_streams = t_new_read_streams;
                f_read_streams[ t_new_read_index ] = new _read_stream( *this );
                return f_read_streams[ t_new_read_index ];
            }

        protected:
            count_t f_length;

        protected:
            class _write_stream :
                public _stream< x_type >
            {
                public:
                    _write_stream( _buffer& p_buffer ) :
                            f_buffer( p_buffer ),
                            f_current_index( 0 ),
                            f_next_index( 0 )
                    {
                    }
                    virtual ~_write_stream()
                    {
                    }

                    count_t operator++( int ) const
                    {
                        if( ++f_next_index == f_buffer.f_length )
                        {
                            f_next_index = 0;
                        }

                        for( count_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                        {
                            f_buffer.f_read_mutexes[ t_index ][ f_next_index ].lock();
                        }

                        for( count_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                        {
                            f_buffer.f_read_mutexes[ t_index ][ f_current_index ].unlock();
                        }

                        f_current_index = f_next_index;

                        return f_current_index;
                    }

                    void state( const state_t& p_state )
                    {
                        f_buffer.f_read_state[ f_current_index ] = p_state;

                        return;
                    }
                    const state_t& state() const
                    {
                        throw error() << "const state on a buffer write stream is not permitted";
                        return stream::s_none;
                    }

                    x_type* data()
                    {
                        return f_buffer.f_read_data[ f_current_index ];
                    }
                    void data( x_type* p_pointer )
                    {
                        f_buffer.f_read_data[ f_current_index ] = p_pointer;
                        return;
                    }

                    const x_type* data() const
                    {
                        throw error() << "const data on a buffer write stream is not permitted";
                        return NULL;
                    }

                private:
                    _buffer& f_buffer;
                    mutable count_t f_current_index;
                    mutable count_t f_next_index;
            };

            state_t f_write_state;
            mutex f_write_mutex;
            _write_stream* f_write_stream;

        protected:
            class _read_stream :
                public _stream< x_type >
            {
                public:
                    _read_stream( _buffer& p_buffer ) :
                            f_buffer( p_buffer ),
                            f_stream_index( f_buffer.f_read_count - 1 ),
                            f_current_index( f_buffer.f_length - 1 ),
                            f_next_index( f_buffer.f_length - 1 )
                    {
                    }
                    virtual ~_read_stream()
                    {
                    }

                    count_t operator++( int ) const
                    {
                        if( ++f_next_index == f_buffer.f_length )
                        {
                            f_next_index = 0;
                        }

                        f_buffer.f_read_mutexes[ f_stream_index ][ f_next_index ].lock();

                        f_buffer.f_read_mutexes[ f_stream_index ][ f_current_index ].unlock();

                        f_current_index = f_next_index;

                        return f_current_index;
                    }

                    void state( const state_t& )
                    {
                        throw error() << "non-const state on a buffer read stream is not permitted";
                        return;
                    }
                    const state_t& state() const
                    {
                        return f_buffer.f_read_state[ f_current_index ];
                    }

                    x_type* data()
                    {
                        throw error() << "non-const data on a buffer read stream is not permitted";
                        return NULL;
                    }
                    void data( x_type * )
                    {
                        throw error() << "non-const data on a buffer read stream is not permitted";
                        return;
                    }

                    const x_type* data() const
                    {
                        return f_buffer.f_read_data[ f_current_index ];
                    }

                private:
                    _buffer& f_buffer;
                    count_t f_stream_index;
                    mutable count_t f_current_index;
                    mutable count_t f_next_index;
            };

            count_t f_read_count;
            state_t* f_read_state;
            x_type** f_read_data;
            mutex** f_read_mutexes;
            _read_stream** f_read_streams;

    };

}

#endif
