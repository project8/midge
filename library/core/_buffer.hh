#ifndef _midge__buffer_hh_
#define _midge__buffer_hh_

#include "midge_error.hh"
#include "_stream.hh"
#include "node.hh"
#include "macros.hh"
#include "coremsg.hh"

#include <mutex>

namespace midge
{

    template< class x_type >
    class _buffer
    {
        public:
            _buffer( node* a_out_node ) :
                    f_length( 0 ),
                    f_out_node( a_out_node ),
                    f_write_stream_name( "out_?" ),
                    f_write_command( stream::s_none ),
                    f_write_mutex(),
                    f_write_stream( NULL ),
                    f_read_count( 0 ),
                    f_read_data( NULL ),
                    f_read_command( NULL ),
                    f_read_mutexes( NULL ),
                    f_mutex_wait_msec( 500 ),
                    f_read_streams( NULL )
            {
            }
            ~_buffer()
            {
            }

        public:
            void set_write_stream_name( const std::string& a_name )
            {
                f_write_stream_name = a_name;
                return;
            }

            void initialize( const count_t& p_length )
            {
                f_length = p_length;
                f_read_command = new enum_t[ f_length ];
                f_read_data = new x_type[ f_length ];
                f_write_stream = new _write_stream( *this );
                f_write_stream->label() = f_out_node->get_name() + ":" + f_write_stream_name;

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
                    (f_read_data[ t_index ].*p_member)( p_1, p_2, p_3, p_4, p_5 );
                }
                return;
            }
            void finalize()
            {
                IF_STREAM_TIMING_ENABLED( f_write_stream->timer_report(); )

                delete f_write_stream;

                delete[] f_read_command;
                delete[] f_read_data;

                for( count_t t_read_index = 0; t_read_index < f_read_count; t_read_index++ )
                {
                    IF_STREAM_TIMING_ENABLED( f_read_streams[ t_read_index ]->timer_report(); )

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
                std::timed_mutex** t_new_read_mutexes = new std::timed_mutex*[ t_new_read_count ];
                _read_stream** t_new_read_streams = new _read_stream*[ t_new_read_count ];

                for( count_t t_read_index = 0; t_read_index < f_read_count; t_read_index++ )
                {
                    t_new_read_mutexes[ t_read_index ] = f_read_mutexes[ t_read_index ];
                    t_new_read_streams[ t_read_index ] = f_read_streams[ t_read_index ];
                }

                f_read_count = t_new_read_count;

                delete[] f_read_mutexes;
                f_read_mutexes = t_new_read_mutexes;
                f_read_mutexes[ t_new_read_index ] = new std::timed_mutex[ f_length ];
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
                            _stream< x_type >(),
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
                        while( ! f_buffer.f_write_mutex.try_lock_for( std::chrono::milliseconds(f_buffer.f_mutex_wait_msec) ) )
                        {
                            if( scarab::cancelable::is_canceled() ) return stream::s_error;
                        }
                        t_command = f_buffer.f_write_command;
                        f_buffer.f_write_mutex.unlock();
                        return t_command;
                    }
                    bool set( enum_t p_command )
                    {
                        if( scarab::cancelable::is_canceled() ) return false;

                        f_buffer.f_read_command[ f_current_index ] = p_command;

                        //coremsg( s_debug ) << "OUT STREAM SET: command <" << p_command << "> set for index " << f_current_index << eom;

                        //count_t t_last_next_index = f_next_index;
                        if( ++f_next_index == f_buffer.f_length )
                        {
                            f_next_index = 0;
                        }

                        IF_STREAM_TIMING_ENABLED( if( p_command == stream::s_run ) this->f_timer.increment_begin() );

                        for( count_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                        {
                            while( ! f_buffer.f_read_mutexes[ t_index ][ f_next_index ].try_lock_for( std::chrono::milliseconds(f_buffer.f_mutex_wait_msec) ) )
                            {
                                if( scarab::cancelable::is_canceled() ) return false;
                            }
                        }

                        IF_STREAM_TIMING_ENABLED( if( p_command == stream::s_run ) this->f_timer.increment_locked() );

                        for( count_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                        {
                            f_buffer.f_read_mutexes[ t_index ][ f_current_index ].unlock();
                        }

                        f_current_index = f_next_index;

                        if( (++f_count % 100000) == 0 )
                        {
                            msg_normal( coremsg, "write stream <" << this << "> processed <" << f_count << "> requests" << eom );
                        }

                        return true;
                    }

                    x_type* data()
                    {
                        return &(f_buffer.f_read_data[ f_current_index ]);
                    }

                    count_t get_current_index() const
                    {
                        return f_current_index;
                    }

                private:
                    _buffer& f_buffer;
                    mutable count_t f_count;
                    mutable count_t f_current_index;
                    mutable count_t f_next_index;
            };

            node* f_out_node;
            std::string f_write_stream_name;
            enum_t f_write_command;
            std::timed_mutex f_write_mutex;
            _write_stream* f_write_stream;

        protected:
            class _read_stream :
                public _stream< x_type >
            {
                public:
                    _read_stream( _buffer& p_buffer ) :
                            _stream< x_type >(),
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
                        if( scarab::cancelable::is_canceled() ) return s_error;

                        if( ++f_next_index == f_buffer.f_length )
                        {
                            f_next_index = 0;
                        }

                        IF_STREAM_TIMING_ENABLED( if( f_buffer.f_read_command[ f_current_index ] == stream::s_run ) this->f_timer.increment_begin(); )

                        //f_buffer.f_read_mutexes[ f_stream_index ][ f_next_index ].lock();
                        while( ! f_buffer.f_read_mutexes[ f_stream_index ][ f_next_index ].try_lock_for( std::chrono::milliseconds(f_buffer.f_mutex_wait_msec) ) )
                        {
                            if( scarab::cancelable::is_canceled() ) return stream::s_error;
                        }

                        IF_STREAM_TIMING_ENABLED( if( f_buffer.f_read_command[ f_current_index ] == stream::s_run ) this->f_timer.increment_locked(); )

                        f_buffer.f_read_mutexes[ f_stream_index ][ f_current_index ].unlock();

                        f_current_index = f_next_index;

                        //coremsg( s_debug ) << "IN STREAM GET: command <" << f_buffer.f_read_command[ f_current_index ] << "> retrieved from index " << f_current_index << eom;

                        return f_buffer.f_read_command[ f_current_index ];
                    }
                    bool set( enum_t p_command )
                    {
                        while( ! f_buffer.f_write_mutex.try_lock_for( std::chrono::milliseconds(f_buffer.f_mutex_wait_msec) ) )
                        {
                            if( scarab::cancelable::is_canceled() ) return false;
                        }
                        f_buffer.f_write_command = p_command;
                        f_buffer.f_write_mutex.unlock();
                        return true;
                    }

                    x_type* data()
                    {
                        return &(f_buffer.f_read_data[ f_current_index ]);
                    }

                    count_t get_current_index() const
                    {
                        return f_current_index;
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
            std::timed_mutex** f_read_mutexes;
            count_t f_mutex_wait_msec;
            _read_stream** f_read_streams;

    };

}

#endif
