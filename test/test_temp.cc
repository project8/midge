#include "types.hh"
#include "mutex.hh"
#include "thread.hh"
#include "error.hh"
#include "message.hh"

#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"

#include <cmath>
#include <unistd.h>

message_declare( testmsg )
message_define( testmsg, test, test )

namespace midge
{
    namespace test
    {
        class stream
        {
            public:
                static const state_t s_started = 0;
                static const state_t s_stopped = 1;

            public:
                stream()
                {
                }
                virtual ~stream()
                {
                }

                virtual void state( state_t p_state ) = 0;
                virtual state_t state() const = 0;
        };

        template< class x_type >
        class read_stream :
            public stream
        {

            public:
                read_stream()
                {
                }
                virtual ~read_stream()
                {
                }

                virtual read_stream< x_type >& operator>>( const x_type*& p_pointer ) = 0;
                virtual read_stream< x_type >& operator<<( const x_type*& p_pointer ) = 0;
        };

        template< class x_type >
        class write_stream :
            public stream
        {
            public:
                write_stream()
                {
                }
                virtual ~write_stream()
                {
                }

                virtual write_stream< x_type >& operator<<( x_type*& p_pointer ) = 0;
                virtual write_stream< x_type >& operator>>( x_type*& p_pointer ) = 0;
        };

        template< class x_type >
        class buffer
        {
            public:
                buffer() :
                        f_guard(),
                        f_state( stream::s_stopped ),
                        f_length( 0 ),
                        f_data( NULL ),
                        f_write_stream( NULL ),
                        f_read_count( 0 ),
                        f_read_guards( NULL ),
                        f_read_streams( NULL )
                {
                }
                ~buffer()
                {
                    for( count_t t_index = 0; t_index < f_length; t_index++ )
                    {
                        delete f_data[ t_index ];
                    }
                    delete[] f_data;

                    delete f_write_stream;
                    for( count_t t_read_index = 0; t_read_index < f_read_count; t_read_index++ )
                    {
                        delete f_read_streams[ t_read_index ];
                        delete[] f_read_guards[ t_read_index ];
                    }
                    delete[] f_read_streams;
                    delete[] f_read_guards;
                }

                void initialize( const count_t& p_length, x_type* (*p_constructor)() )
                {
                    f_length = p_length;
                    f_data = new x_type*[ p_length ];
                    for( count_t t_index = 0; t_index < p_length; t_index++ )
                    {
                        f_data[ t_index ] = (*p_constructor)();
                    }
                    return;
                }

                void state( state_t p_state )
                {
                    f_guard.lock();
                    f_state = p_state;
                    f_guard.unlock();
                    return;
                }
                state_t state() const
                {
                    state_t t_state;
                    f_guard.lock();
                    t_state = f_state;
                    f_guard.unlock();
                    return t_state;
                }

                write_stream< x_type >* write()
                {
                    f_write_stream = new buffer_write_stream( this );
                    return f_write_stream;
                }
                read_stream< x_type >* read()
                {
                    count_t t_new_read_index = f_read_count;
                    count_t t_new_read_count = f_read_count + 1;
                    mutex** t_new_read_guards = new mutex*[ t_new_read_count ];
                    buffer_read_stream** t_new_read_streams = new buffer_read_stream*[ t_new_read_count ];

                    for( count_t t_index = 0; t_index < f_read_count; t_index++ )
                    {
                        t_new_read_guards[ t_index ] = f_read_guards[ t_index ];
                        t_new_read_streams[ t_index ] = f_read_streams[ t_index ];
                    }

                    f_read_count = t_new_read_count;

                    delete[] f_read_guards;
                    f_read_guards = t_new_read_guards;
                    f_read_guards[ t_new_read_index ] = new mutex[ f_length ];
                    f_read_guards[ t_new_read_index ][ 0 ].lock();

                    delete[] f_read_streams;
                    f_read_streams = t_new_read_streams;
                    f_read_streams[ t_new_read_index ] = new buffer_read_stream( this, t_new_read_index );
                    return f_read_streams[ t_new_read_index ];
                }

            protected:
                mutable mutex f_guard;
                state_t f_state;
                count_t f_length;
                x_type** f_data;

            protected:
                class buffer_write_stream :
                    public write_stream< x_type >
                {
                    public:
                        buffer_write_stream( buffer* p_buffer ) :
                                f_buffer( *p_buffer ),
                                f_current_index( 0 ),
                                f_next_index( 0 )
                        {
                        }
                        virtual ~buffer_write_stream()
                        {
                        }

                        void state( state_t p_state )
                        {
                            f_buffer.f_guard.lock();
                            f_buffer.f_state = p_state;
                            f_buffer.f_guard.unlock();
                        }
                        state_t state() const
                        {
                            state_t t_state;
                            f_buffer.f_guard.lock();
                            t_state = f_buffer.f_state;
                            f_buffer.f_guard.unlock();
                            return t_state;
                        }

                        write_stream< x_type >& operator>>( x_type*& p_pointer )
                        {
                            if( ++f_next_index == f_buffer.f_length )
                            {
                                f_next_index = 0;
                            }

                            for( count_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                            {
                                f_buffer.f_read_guards[ t_index ][ f_next_index ].lock();
                            }

                            p_pointer = f_buffer.f_data[ f_current_index ];

                            return *this;
                        }
                        write_stream< x_type >& operator<<( x_type*& )
                        {
                            for( count_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                            {
                                f_buffer.f_read_guards[ t_index ][ f_current_index ].unlock();
                            }

                            f_current_index = f_next_index;

                            return *this;
                        }

                    private:
                        buffer& f_buffer;
                        count_t f_current_index;
                        count_t f_next_index;
                };

                buffer_write_stream* f_write_stream;

            protected:
                class buffer_read_stream :
                    public read_stream< x_type >
                {
                    public:
                        buffer_read_stream( buffer* p_buffer, const count_t& p_index ) :
                                f_buffer( *p_buffer ),
                                f_stream_index( p_index ),
                                f_current_index( 0 )
                        {
                        }
                        virtual ~buffer_read_stream()
                        {
                        }

                        void state( state_t p_state )
                        {
                            f_buffer.f_guard.lock();
                            f_buffer.f_state = p_state;
                            f_buffer.f_guard.unlock();
                        }
                        state_t state() const
                        {
                            state_t t_state;
                            f_buffer.f_guard.lock();
                            t_state = f_buffer.f_state;
                            f_buffer.f_guard.unlock();
                            return t_state;
                        }

                        read_stream< x_type >& operator>>( const x_type*& p_pointer )
                        {
                            f_buffer.f_read_guards[ f_stream_index ][ f_current_index ].lock();

                            p_pointer = f_buffer.f_data[ f_current_index ];

                            return *this;
                        }
                        read_stream< x_type >& operator<<( const x_type*& )
                        {
                            f_buffer.f_read_guards[ f_stream_index ][ f_current_index ].unlock();

                            if( ++f_current_index == f_buffer.f_length )
                            {
                                f_current_index = 0;
                            }

                            return *this;
                        }

                    private:
                        buffer& f_buffer;
                        count_t f_stream_index;
                        count_t f_current_index;
                };

                count_t f_read_count;
                mutex** f_read_guards;
                buffer_read_stream** f_read_streams;

        };

        class writer
        {
            public:
                writer( buffer< real_t >* p_buffer, const count_t& p_seed ) :
                        f_stream( *(p_buffer->write()) ),
                        f_seed( p_seed ),
                        f_rng( gsl_rng_alloc( gsl_rng_mt19937 ) )
                {
                    gsl_rng_set( f_rng, f_seed );
                }
                ~writer()
                {
                    gsl_rng_free( f_rng );
                }

                void execute()
                {
                    double* t_value;
                    while( true )
                    {
                        f_stream >> t_value;
                        (*t_value) = gsl_ran_flat( f_rng, 0., 10. );

                        if( f_stream.state() == stream::s_stopped )
                        {
                            testmsg( s_normal ) << "writer <" << f_seed << "> stopping" << eom;
                            f_stream << t_value;
                            break;
                        }

                        if( gsl_ran_flat( f_rng, 0., 20. ) < 1. )
                        {
                            testmsg( s_normal ) << "writer <" << f_seed << "> initiates stop" << eom;
                            f_stream.state( stream::s_stopped );
                            f_stream << t_value;
                            break;
                        }

                        testmsg( s_normal ) << "writer <" << f_seed << "> pushing a value of <" << *t_value << ">" << eom;
                        f_stream << t_value;
                        usleep( 1000000 );
                    }
                    return;
                }

            private:
                write_stream< real_t >& f_stream;
                count_t f_seed;
                gsl_rng* f_rng;
        };

        class reader
        {
            public:
                reader( buffer< real_t >* p_buffer, const count_t& p_seed ) :
                        f_stream( *(p_buffer->read()) ),
                        f_seed( p_seed ),
                        f_rng( gsl_rng_alloc( gsl_rng_mt19937 ) )
                {
                    gsl_rng_set( f_rng, f_seed );
                }
                ~reader()
                {
                    gsl_rng_free( f_rng );
                }

                void execute()
                {
                    const double* t_value;
                    while( true )
                    {
                        f_stream >> t_value;
                        if( f_stream.state() == stream::s_stopped )
                        {
                            testmsg( s_normal ) << "reader <" << f_seed << "> stopping" << eom;
                            f_stream << t_value;
                            break;
                        }

                        if( gsl_ran_flat( f_rng, 0., 20. ) < 1. )
                        {
                            testmsg( s_normal ) << "reader <" << f_seed << "> initiates stop" << eom;
                            f_stream.state( stream::s_stopped );
                            f_stream << t_value;
                            break;
                        }

                        testmsg( s_normal ) << "  reader <" << f_seed << "> pulling a value of <" << *t_value << ">" << eom;
                        f_stream << t_value;
                    }
                    return;
                }

            private:
                read_stream< real_t >& f_stream;
                count_t f_seed;
                gsl_rng* f_rng;
        };

        real_t* new_real()
        {
            real_t* t_real = new real_t;
            (*t_real) = 0.;
            return t_real;
        }
    }
}

using namespace midge;
using namespace midge::test;

int main()
{
    buffer< real_t > t_buffer;
    t_buffer.initialize( 10, &new_real );

    writer t_dan_writer( &t_buffer, 51385 );
    reader t_katie_reader( &t_buffer, 82284 );
    reader t_erin_reader( &t_buffer, 112383 );
    reader t_susanne_reader( &t_buffer, 53083 );
    reader t_rose_reader( &t_buffer, 31387 );

    thread t_dan_thread, t_katie_thread, t_erin_thread, t_susanne_thread, t_rose_thread;
    t_dan_thread.start( &t_dan_writer, &writer::execute );
    t_katie_thread.start( &t_katie_reader, &reader::execute );
    t_erin_thread.start( &t_erin_reader, &reader::execute );
    t_susanne_thread.start( &t_susanne_reader, &reader::execute );
    t_rose_thread.start( &t_rose_reader, &reader::execute );

    t_buffer.state( stream::s_started );

    t_dan_thread.start();
    t_katie_thread.start();
    t_erin_thread.start();
    t_susanne_thread.start();
    t_rose_thread.start();

    t_dan_thread.join();
    t_katie_thread.join();
    t_erin_thread.join();
    t_susanne_thread.join();
    t_rose_thread.join();

    return 0;
}
