#include <random>

#include <cmath>
#include <unistd.h>
#include <mutex>

#include "midge_error.hh"
#include "message.hh"
#include "thread.hh"
#include "types.hh"

message_declare( testmsg )
message_define( testmsg, test, test )

namespace midge
{
    namespace test
    {
        class stream
        {
            public:
                static const enum_t s_idle;
                static const enum_t s_started;
                static const enum_t s_stopped;

            public:
                stream()
                {
                }
                virtual ~stream()
                {
                }

                virtual stream& operator>( enum_t& p_state ) = 0;
                virtual stream& operator<( const enum_t& p_state ) = 0;
        };

        const enum_t stream::s_idle = 0;
        const enum_t stream::s_started = 1;
        const enum_t stream::s_stopped = 2;

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

                virtual write_stream< x_type >& operator>>( x_type*& p_pointer ) = 0;
                virtual write_stream< x_type >& operator<<( x_type*& p_pointer ) = 0;
        };

        template< class x_type >
        class buffer
        {
            public:
                buffer() :
                        f_length( 0 ),
                        f_write_state( stream::s_idle ),
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
                ~buffer()
                {
                    delete f_write_stream;

                    for( index_t t_index = 0; t_index < f_length; t_index++ )
                    {
                        delete f_read_state[ t_index ];
                    }
                    delete[] f_read_state;

                    for( index_t t_index = 0; t_index < f_length; t_index++ )
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

                void initialize( const count_t& p_length, enum_t* (p_state_factory)(), x_type* (*p_data_factory)() )
                {
                    f_length = p_length;
                    f_read_state = new enum_t*[ p_length ];
                    f_read_data = new x_type*[ p_length ];
                    for( index_t t_index = 0; t_index < p_length; t_index++ )
                    {
                        f_read_state[ t_index ] = (*p_state_factory)();
                        f_read_data[ t_index ] = (*p_data_factory)();
                    }
                    return;
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
                    std::mutex** t_new_read_state_mutexes = new std::mutex*[ t_new_read_count ];
                    std::mutex** t_new_read_data_mutexes = new std::mutex*[ t_new_read_count ];
                    buffer_read_stream** t_new_read_streams = new buffer_read_stream*[ t_new_read_count ];

                    for( index_t t_index = 0; t_index < f_read_count; t_index++ )
                    {
                        t_new_read_state_mutexes[ t_index ] = f_read_state_mutexes[ t_index ];
                        t_new_read_data_mutexes[ t_index ] = f_read_data_mutexes[ t_index ];
                        t_new_read_streams[ t_index ] = f_read_streams[ t_index ];
                    }

                    f_read_count = t_new_read_count;

                    delete[] f_read_state_mutexes;
                    f_read_state_mutexes = t_new_read_state_mutexes;
                    f_read_state_mutexes[ t_new_read_index ] = new std::mutex[ f_length ];
                    f_read_state_mutexes[ t_new_read_index ][ 0 ].lock();

                    delete[] f_read_data_mutexes;
                    f_read_data_mutexes = t_new_read_data_mutexes;
                    f_read_data_mutexes[ t_new_read_index ] = new std::mutex[ f_length ];
                    f_read_data_mutexes[ t_new_read_index ][ 0 ].lock();

                    delete[] f_read_streams;
                    f_read_streams = t_new_read_streams;
                    f_read_streams[ t_new_read_index ] = new buffer_read_stream( this );
                    return f_read_streams[ t_new_read_index ];
                }

            protected:
                count_t f_length;

            protected:
                class buffer_write_stream :
                    public write_stream< x_type >
                {
                    public:
                        buffer_write_stream( buffer* p_buffer ) :
                                f_buffer( *p_buffer ),
                                f_current_state_index( 0 ),
                                f_next_state_index( 0 ),
                                f_current_data_index( 0 ),
                                f_next_data_index( 0 )
                        {
                        }
                        virtual ~buffer_write_stream()
                        {
                        }

                        write_stream< x_type >& operator>( enum_t& p_state )
                        {
                            f_buffer.f_write_state_mutex.lock();
                            p_state = f_buffer.f_write_state;
                            f_buffer.f_write_state_mutex.unlock();

                            return *this;
                        }
                        write_stream< x_type >& operator<( const enum_t& p_state )
                        {
                            if( ++f_next_state_index == f_buffer.f_length )
                            {
                                f_next_state_index = 0;
                            }

                            for( index_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                            {
                                f_buffer.f_read_state_mutexes[ t_index ][ f_next_state_index ].lock();
                            }

                            *(f_buffer.f_read_state[ f_current_state_index ]) = p_state;

                            for( index_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                            {
                                f_buffer.f_read_state_mutexes[ t_index ][ f_current_state_index ].unlock();
                            }

                            f_current_state_index = f_next_state_index;

                            return *this;
                        }
                        write_stream< x_type >& operator>>( x_type*& p_pointer )
                        {
                            if( ++f_next_data_index == f_buffer.f_length )
                            {
                                f_next_data_index = 0;
                            }

                            for( index_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                            {
                                f_buffer.f_read_data_mutexes[ t_index ][ f_next_data_index ].lock();
                            }

                            p_pointer = f_buffer.f_read_data[ f_current_data_index ];

                            return *this;
                        }
                        write_stream< x_type >& operator<<( x_type*& )
                        {
                            for( index_t t_index = 0; t_index < f_buffer.f_read_count; t_index++ )
                            {
                                f_buffer.f_read_data_mutexes[ t_index ][ f_current_data_index ].unlock();
                            }

                            f_current_data_index = f_next_data_index;

                            return *this;
                        }

                    private:
                        buffer& f_buffer;
                        count_t f_current_state_index;
                        count_t f_next_state_index;
                        count_t f_current_data_index;
                        count_t f_next_data_index;
                };

                enum_t f_write_state;
                std::mutex f_write_state_mutex;
                buffer_write_stream* f_write_stream;

            protected:
                class buffer_read_stream :
                    public read_stream< x_type >
                {
                    public:
                        buffer_read_stream( buffer* p_buffer ) :
                                f_buffer( *p_buffer ),
                                f_stream_index( f_buffer.f_read_count - 1 ),
                                f_current_state_index( 0 ),
                                f_current_data_index( 0 )
                        {
                        }
                        virtual ~buffer_read_stream()
                        {
                        }

                        read_stream< x_type >& operator>( enum_t& p_state )
                        {
                            f_buffer.f_read_state_mutexes[ f_stream_index ][ f_current_state_index ].lock();

                            p_state = *(f_buffer.f_read_state[ f_current_state_index ]);

                            f_buffer.f_read_state_mutexes[ f_stream_index ][ f_current_state_index ].unlock();

                            if( ++f_current_state_index == f_buffer.f_length )
                            {
                                f_current_state_index = 0;
                            }

                            return *this;
                        }
                        read_stream< x_type >& operator<( const enum_t& p_state )
                        {
                            f_buffer.f_write_state_mutex.lock();
                            f_buffer.f_write_state = p_state;
                            f_buffer.f_write_state_mutex.unlock();

                            return *this;
                        }
                        read_stream< x_type >& operator>>( const x_type*& p_pointer )
                        {
                            f_buffer.f_read_data_mutexes[ f_stream_index ][ f_current_data_index ].lock();

                            p_pointer = f_buffer.f_read_data[ f_current_data_index ];

                            return *this;
                        }
                        read_stream< x_type >& operator<<( const x_type*& )
                        {
                            f_buffer.f_read_data_mutexes[ f_stream_index ][ f_current_data_index ].unlock();

                            if( ++f_current_data_index == f_buffer.f_length )
                            {
                                f_current_data_index = 0;
                            }

                            return *this;
                        }

                    private:
                        buffer& f_buffer;
                        count_t f_stream_index;
                        count_t f_current_state_index;
                        count_t f_current_data_index;
                };

                count_t f_read_count;
                enum_t** f_read_state;
                std::mutex** f_read_state_mutexes;
                x_type** f_read_data;
                std::mutex** f_read_data_mutexes;
                buffer_read_stream** f_read_streams;

        };

        class writer
        {
            public:
                writer( buffer< real_t >* p_buffer, const count_t& p_seed ) :
                        f_stream( *(p_buffer->write()) ),
                        f_seed( p_seed ),
                        f_rng( p_seed )
                {}
                ~writer()
                {}

                void execute()
                {

                    count_t t_count = 0;
                    count_t t_sleep;
                    enum_t t_state;
                    double* t_value;
                    std::uniform_real_distribution<> t_dist_uniform_0_20( 0., 20. );
                    std::uniform_real_distribution<> t_dist_uniform_0_10( 0., 10. );
                    std::uniform_real_distribution<> t_dist_uniform_50k_150k( 50000., 150000. );
                    while( true )
                    {
                        f_stream > t_state;
                        testmsg( s_normal ) << "writer <" << f_seed << "> pulling a state of <" << t_state << "> at <" << t_count << ">" << eom;
                        if( t_state == stream::s_stopped )
                        {
                            testmsg( s_normal ) << "writer <" << f_seed << "> stopping" << eom;
                            f_stream < stream::s_stopped;
                            break;
                        }
                        if( t_dist_uniform_0_20( f_rng ) < 1. )
                        {
                            testmsg( s_normal ) << "writer <" << f_seed << "> initiates stop" << eom;
                            f_stream < stream::s_stopped;
                            break;
                        }
                        f_stream < stream::s_started;

                        f_stream >> t_value;
                        (*t_value) = t_dist_uniform_0_10( f_rng );
                        testmsg( s_normal ) << "writer <" << f_seed << "> pushing a value of <" << *t_value << "> at <" << t_count << ">" << eom;
                        f_stream << t_value;
                        t_sleep = (count_t) (round(t_dist_uniform_50k_150k( f_rng )));
                        usleep( t_sleep );
                        t_count++;
                    }
                    return;
                }

            private:
                write_stream< real_t >& f_stream;
                count_t f_seed;
                std::mt19937 f_rng;
        };

        class reader
        {
            public:
                reader( buffer< real_t >* p_buffer, const count_t& p_seed ) :
                        f_stream( *(p_buffer->read()) ),
                        f_seed( p_seed ),
                        f_rng( p_seed )
                {}
                ~reader()
                {}

                void execute()
                {
                    count_t t_count = 0;
                    count_t t_sleep;
                    enum_t t_state;
                    const double* t_value;
                    std::uniform_real_distribution<> t_dist_uniform_0_20( 0., 20. );
                    std::uniform_real_distribution<> t_dist_uniform_200k_600k( 200000., 600000. );
                    while( true )
                    {

                        f_stream > t_state;
                        testmsg( s_normal ) << "  reader <" << f_seed << "> pulling a state of <" << t_state << "> at <" << t_count << ">" << eom;
                        if( t_state == stream::s_stopped )
                        {
                            testmsg( s_normal ) << "  reader <" << f_seed << "> stopping" << eom;
                            f_stream < stream::s_stopped;
                            break;
                        }

                        if( t_dist_uniform_0_20( f_rng ) < 1. )
                        {
                            testmsg( s_normal ) << "  reader <" << f_seed << "> initiates stop" << eom;
                            f_stream < stream::s_stopped;
                            break;
                        }
                        //f_stream < stream::s_started;

                        f_stream >> t_value;
                        testmsg( s_normal ) << "  reader <" << f_seed << "> pulling a value of <" << *t_value << "> at <" << t_count << ">" << eom;
                        f_stream << t_value;
                        t_sleep = (count_t) (round(t_dist_uniform_200k_600k( f_rng )));
                        usleep( t_sleep );
                        t_count++;
                    }
                    return;
                }

            private:
                read_stream< real_t >& f_stream;
                count_t f_seed;
                std::mt19937 f_rng;
        };

        real_t* new_real()
        {
            real_t* t_real = new real_t;
            (*t_real) = 0.;
            return t_real;
        }
        enum_t* new_state()
        {
            enum_t* t_state = new enum_t;
            (*t_state) = stream::s_idle;
            return t_state;
        }
    }
}

using namespace midge;
using namespace midge::test;

int main()
{
    buffer< real_t > t_buffer;
    t_buffer.initialize( 10, &new_state, &new_real );

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
