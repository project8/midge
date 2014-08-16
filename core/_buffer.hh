#ifndef _midge__buffer_hh_
#define _midge__buffer_hh_

#include "_stream.hh"
#include "error.hh"

namespace midge
{

    template< class x_type >
    class _buffer
    {
        public:
            _buffer() :
                    f_length( 0 ),
                    f_write_state( stream::s_none ),
                    f_write_state_mutex(),
                    f_write_stream( NULL ),
                    f_read_count( 0 ),
                    f_read_state( NULL ),
                    f_read_state_mutexes( NULL ),
                    f_read_data( NULL ),
                    f_read_data_mutexes( NULL ),
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
                    delete[] f_read_state_mutexes[ t_read_index ];
                    delete[] f_read_data_mutexes[ t_read_index ];
                    delete f_read_streams[ t_read_index ];
                }
                delete[] f_read_state_mutexes;
                delete[] f_read_data_mutexes;
                delete[] f_read_streams;
            }

            void initialize( const count_t& p_length )
            {
                coremsg( s_warning ) << "buffer initializing to length <" << p_length << ">" << eom;

                f_length = p_length;
                f_read_state = new state_t[ f_length ];
                f_read_data = new x_type*[ f_length ];
                for( count_t t_index = 0; t_index < f_length; t_index++ )
                {
                    f_read_state[ t_index ] = stream::s_none;
                    f_read_data[ t_index ] = new x_type();
                }

                coremsg( s_warning ) << "buffer initialized to length <" << f_length << ">" << eom;

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
                mutex** t_new_read_state_mutexes = new mutex*[ t_new_read_count ];
                mutex** t_new_read_data_mutexes = new mutex*[ t_new_read_count ];
                _read_stream** t_new_read_streams = new _read_stream*[ t_new_read_count ];

                for( count_t t_index = 0; t_index < f_read_count; t_index++ )
                {
                    t_new_read_state_mutexes[ t_index ] = f_read_state_mutexes[ t_index ];
                    t_new_read_data_mutexes[ t_index ] = f_read_data_mutexes[ t_index ];
                    t_new_read_streams[ t_index ] = f_read_streams[ t_index ];
                }

                f_read_count = t_new_read_count;

                delete[] f_read_state_mutexes;
                f_read_state_mutexes = t_new_read_state_mutexes;
                f_read_state_mutexes[ t_new_read_index ] = new mutex[ f_length ];
                f_read_state_mutexes[ t_new_read_index ][ 0 ].lock();

                delete[] f_read_data_mutexes;
                f_read_data_mutexes = t_new_read_data_mutexes;
                f_read_data_mutexes[ t_new_read_index ] = new mutex[ f_length ];
                f_read_data_mutexes[ t_new_read_index ][ 0 ].lock();

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
                            f_current_state_index( 0 ),
                            f_next_state_index( 0 ),
                            f_current_data_index( 0 ),
                            f_next_data_index( 0 )
                    {
                    }
                    virtual ~_write_stream()
                    {
                    }

                    const _stream< x_type >& operator>( state_t& p_state ) const
                    {
                        f_buffer.f_write_state_mutex.lock();
                        p_state = f_buffer.f_write_state;
                        f_buffer.f_write_state_mutex.unlock();

                        return *this;
                    }
                    const _stream< x_type >& operator<( const state_t& p_state ) const
                    {
                        if( ++f_next_state_index == f_buffer.f_length )
                        {
                            f_next_state_index = 0;
                        }

                        for( count_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                        {
                            f_buffer.f_read_state_mutexes[ t_index ][ f_next_state_index ].lock();
                        }

                        f_buffer.f_read_state[ f_current_state_index ] = p_state;

                        for( count_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                        {
                            f_buffer.f_read_state_mutexes[ t_index ][ f_current_state_index ].unlock();
                        }

                        f_current_state_index = f_next_state_index;

                        return *this;
                    }
                    _stream< x_type >& operator>>( x_type*& p_pointer )
                    {
                        if( ++f_next_data_index == f_buffer.f_length )
                        {
                            f_next_data_index = 0;
                        }

                        for( count_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                        {
                            f_buffer.f_read_data_mutexes[ t_index ][ f_next_data_index ].lock();
                        }

                        p_pointer = f_buffer.f_read_data[ f_current_data_index ];

                        return *this;
                    }
                    _stream< x_type >& operator<<( x_type*& )
                    {
                        for( count_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                        {
                            f_buffer.f_read_data_mutexes[ t_index ][ f_current_data_index ].unlock();
                        }

                        f_current_data_index = f_next_data_index;

                        return *this;
                    }
                    _stream< x_type >& operator++( int )
                    {
                        if( ++f_next_data_index == f_buffer.f_length )
                        {
                            f_next_data_index = 0;
                        }

                        for( count_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                        {
                            f_buffer.f_read_data_mutexes[ t_index ][ f_next_data_index ].lock();
                        }

                        for( count_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                        {
                            f_buffer.f_read_data_mutexes[ t_index ][ f_current_data_index ].unlock();
                        }

                        f_current_data_index = f_next_data_index;

                        return *this;
                    }
                    const _stream< x_type >& operator>>( const x_type*& ) const
                    {
                        throw error() << "const removals from a buffer write stream are not permitted";
                        return *this;
                    }
                    const _stream< x_type >& operator<<( const x_type*& ) const
                    {
                        throw error() << "const insertions to a buffer write stream are not permitted";
                        return *this;
                    }
                    const _stream< x_type >& operator++( int ) const
                    {
                        throw error() << "cont incrementations to a buffer write stream are not permitted";
                    }

                private:
                    _buffer& f_buffer;
                    mutable count_t f_current_state_index;
                    mutable count_t f_next_state_index;
                    mutable count_t f_current_data_index;
                    mutable count_t f_next_data_index;
            };

            state_t f_write_state;
            mutex f_write_state_mutex;
            _write_stream* f_write_stream;

        protected:
            class _read_stream :
                public _stream< x_type >
            {
                public:
                    _read_stream( _buffer& p_buffer ) :
                            f_buffer( p_buffer ),
                            f_stream_index( f_buffer.f_read_count - 1 ),
                            f_current_state_index( 0 ),
                            f_current_data_index( 0 )
                    {
                    }
                    virtual ~_read_stream()
                    {
                    }

                    const _stream< x_type >& operator>( state_t& p_state ) const
                    {
                        f_buffer.f_read_state_mutexes[ f_stream_index ][ f_current_state_index ].lock();

                        p_state = f_buffer.f_read_state[ f_current_state_index ];

                        f_buffer.f_read_state_mutexes[ f_stream_index ][ f_current_state_index ].unlock();

                        if( ++f_current_state_index == f_buffer.f_length )
                        {
                            f_current_state_index = 0;
                        }

                        return *this;
                    }
                    const _stream< x_type >& operator<( const state_t& p_state ) const
                    {
                        f_buffer.f_write_state_mutex.lock();
                        f_buffer.f_write_state = p_state;
                        f_buffer.f_write_state_mutex.unlock();

                        return *this;
                    }
                    _stream< x_type >& operator>>( x_type*& )
                    {
                        throw error() << "non-const removals from a buffer read stream are not permitted";
                        return *this;
                    }
                    _stream< x_type >& operator<<( x_type*& )
                    {
                        throw error() << "non-const insertions to a buffer read stream are not permitted";
                        return *this;
                    }
                    _stream< x_type >& operator++( int )
                    {
                        throw error() << "non-const incrementations to a buffer read stream are not permitted";
                        return *this;
                    }
                    const _stream< x_type >& operator>>( const x_type*& p_pointer ) const
                    {
                        f_buffer.f_read_data_mutexes[ f_stream_index ][ f_current_data_index ].lock();

                        p_pointer = f_buffer.f_read_data[ f_current_data_index ];

                        return *this;
                    }
                    const _stream< x_type >& operator<<( const x_type*& ) const
                    {
                        f_buffer.f_read_data_mutexes[ f_stream_index ][ f_current_data_index ].unlock();

                        if( ++f_current_data_index == f_buffer.f_length )
                        {
                            f_current_data_index = 0;
                        }

                        return *this;
                    }
                    const _stream< x_type >& operator++( int ) const
                    {
                        f_buffer.f_read_data_mutexes[ f_stream_index ][ f_current_data_index ].lock();

                        f_buffer.f_read_data_mutexes[ f_stream_index ][ f_current_data_index ].unlock();

                        if( ++f_current_data_index == f_buffer.f_length )
                        {
                            f_current_data_index = 0;
                        }

                        return *this;
                    }

                private:
                    _buffer& f_buffer;
                    count_t f_stream_index;
                    mutable count_t f_current_state_index;
                    mutable count_t f_current_data_index;
            };

            count_t f_read_count;
            state_t* f_read_state;
            mutex** f_read_state_mutexes;
            x_type** f_read_data;
            mutex** f_read_data_mutexes;
            _read_stream** f_read_streams;

    };

}

#endif
