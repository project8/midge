#ifndef _midge__buffer_hh_
#define _midge__buffer_hh_

#include "midge_error.hh"
#include "_stream.hh"
#include "macros.hh"
#include "coremsg.hh"

#include <mutex>

namespace midge
{

    template< class x_type >
    class _buffer
    {
        public:
            _buffer() :
                    f_length( 0 ),
                    f_write_command( stream::s_none ),
                    f_write_mutex(),
                    f_write_stream( NULL ),
                    f_read_count( 0 ),
                    f_read_data( NULL ),
                    f_read_command( NULL ),
                    f_read_mutexes( NULL ),
                    f_read_streams( NULL )
            {
            }
            ~_buffer()
            {
            }

        public:
            void initialize( const count_t& p_length )
            {
                f_length = p_length;
                f_read_command = new enum_t[ f_length ];
                f_read_data = new x_type[ f_length ];
                f_write_stream = new _write_stream( *this );

                return;
            }
            template< class x_r >
            void call( x_r (x_type::*p_member)() )
            {
                for( index_t t_index = 0; t_index < f_length; t_index++ )
                {
                    (f_read_data[ t_index ].*p_member)();
                }
                return;
            }
            template< class x_r, class x_a1, class x_p1 >
            void call( x_r (x_type::*p_member)( x_a1 ), x_p1 p_1 )
            {
                for( index_t t_index = 0; t_index < f_length; t_index++ )
                {
                    (f_read_data[ t_index ].*p_member)( p_1 );
                }
                return;
            }
            template< class x_r, class x_a1, class x_p1, class x_a2, class x_p2 >
            void call( x_r (x_type::*p_member)( x_a1, x_a2 ), x_p1 p_1, x_p2 p_2 )
            {
                for( index_t t_index = 0; t_index < f_length; t_index++ )
                {
                    (f_read_data[ t_index ].*p_member)( p_1, p_2 );
                }
                return;
            }
            template< class x_r, class x_a1, class x_p1, class x_a2, class x_p2, class x_a3, class x_p3 >
            void call( x_r (x_type::*p_member)( x_a1, x_a2, x_a3 ), x_p1 p_1, x_p2 p_2, x_p3 p_3 )
            {
                for( index_t t_index = 0; t_index < f_length; t_index++ )
                {
                    (f_read_data[ t_index ].*p_member)( p_1, p_2, p_3 );
                }
                return;
            }
            template< class x_r, class x_a1, class x_p1, class x_a2, class x_p2, class x_a3, class x_p3, class x_a4, class x_p4 >
            void call( x_r (x_type::*p_member)( x_a1, x_a2, x_a3, x_a4 ), x_p1 p_1, x_p2 p_2, x_p3 p_3, x_p4 p_4 )
            {
                for( index_t t_index = 0; t_index < f_length; t_index++ )
                {
                    (f_read_data[ t_index ].*p_member)( p_1, p_2, p_3, p_4 );
                }
                return;
            }
            template< class x_r, class x_a1, class x_p1, class x_a2, class x_p2, class x_a3, class x_p3, class x_a4, class x_p4, class x_a5, class x_p5 >
            void call( x_r (x_type::*p_member)( x_a1, x_a2, x_a3, x_a4, x_a5 ), x_p1 p_1, x_p2 p_2, x_p3 p_3, x_p4 p_4, x_p5 p_5 )
            {
                for( index_t t_index = 0; t_index < f_length; t_index++ )
                {
                    (f_read_data[ t_index ].*p_member)( p_1, p_2, p_3, p_5 );
                }
                return;
            }
            void finalize()
            {
                delete f_write_stream;

                delete[] f_read_command;
                delete[] f_read_data;

                for( count_t t_read_index = 0; t_read_index < f_read_count; t_read_index++ )
                {
                    delete[] f_read_mutexes[ t_read_index ];
                    delete f_read_streams[ t_read_index ];
                }
                delete[] f_read_mutexes;
                delete[] f_read_streams;
            }

            _stream< x_type >* write()
            {
                return f_write_stream;
            }
            _stream< x_type >* read()
            {
                count_t t_new_read_index = f_read_count;
                count_t t_new_read_count = f_read_count + 1;
                std::mutex** t_new_read_mutexes = new std::mutex*[ t_new_read_count ];
                _read_stream** t_new_read_streams = new _read_stream*[ t_new_read_count ];

                for( count_t t_read_index = 0; t_read_index < f_read_count; t_read_index++ )
                {
                    t_new_read_mutexes[ t_read_index ] = f_read_mutexes[ t_read_index ];
                    t_new_read_streams[ t_read_index ] = f_read_streams[ t_read_index ];
                }

                f_read_count = t_new_read_count;

                delete[] f_read_mutexes;
                f_read_mutexes = t_new_read_mutexes;
                f_read_mutexes[ t_new_read_index ] = new std::mutex[ f_length ];
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
                            f_count( 0 ),
                            f_current_index( 0 ),
                            f_next_index( 0 )
                    {
                    }
                    virtual ~_write_stream()
                    {
                    }

                    enum_t get()
                    {
                        enum_t t_command;
                        f_buffer.f_write_mutex.lock();
                        t_command = f_buffer.f_write_command;
                        f_buffer.f_write_mutex.unlock();
                        return t_command;
                    }
                    void set( enum_t p_command )
                    {
                        f_buffer.f_read_command[ f_current_index ] = p_command;

                        if( (++f_count % 1000) == 0 )
                        {
                            coremsg( s_normal ) << "write stream <" << this << "> processed <" << f_count << "> requests" << eom;
                        }

                        if( ++f_next_index == f_buffer.f_length )
                        {
                            f_next_index = 0;
                        }

                        for( index_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                        {
                            f_buffer.f_read_mutexes[ t_index ][ f_next_index ].lock();
                        }

                        for( index_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                        {
                            f_buffer.f_read_mutexes[ t_index ][ f_current_index ].unlock();
                        }

                        f_current_index = f_next_index;

                        return;
                    }

                    x_type* data()
                    {
                        return &(f_buffer.f_read_data[ f_current_index ]);
                    }

                private:
                    _buffer& f_buffer;
                    mutable count_t f_count;
                    mutable count_t f_current_index;
                    mutable count_t f_next_index;
            };

            enum_t f_write_command;
            std::mutex f_write_mutex;
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

                    enum_t get()
                    {
                        if( ++f_next_index == f_buffer.f_length )
                        {
                            f_next_index = 0;
                        }

                        f_buffer.f_read_mutexes[ f_stream_index ][ f_next_index ].lock();

                        f_buffer.f_read_mutexes[ f_stream_index ][ f_current_index ].unlock();

                        f_current_index = f_next_index;

                        return f_buffer.f_read_command[ f_current_index ];
                    }
                    void set( enum_t p_command )
                    {
                        f_buffer.f_write_mutex.lock();
                        f_buffer.f_write_command = p_command;
                        f_buffer.f_write_mutex.unlock();
                        return;
                    }

                    x_type* data()
                    {
                        return &(f_buffer.f_read_data[ f_current_index ]);
                    }

                private:
                    _buffer& f_buffer;
                    count_t f_stream_index;
                    mutable count_t f_current_index;
                    mutable count_t f_next_index;
            };

            count_t f_read_count;
            x_type* f_read_data;
            enum_t* f_read_command;
            std::mutex** f_read_mutexes;
            _read_stream** f_read_streams;

    };

}

#endif
