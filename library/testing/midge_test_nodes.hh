#ifndef _midge_test_nodes_hh_
#define _midge_test_nodes_hh_

#include "consumer.hh"
#include "diptera.hh"
#include "producer.hh"

#include <atomic>
#include <cstddef>
#include <functional>
#include <string>

namespace midge
{
    namespace testing
    {
        /*!
         @class test_producer
         @brief Producer that emits a fixed number of records built by a caller-supplied
                fill function, then stops and exits its output stream.

         @details
         The fill function writes *in place* into the buffer slot it is handed:
             void fill( x_type* a_slot, std::size_t a_index )
         It must never copy or assign x_type -- midge payload types commonly hold raw
         owning pointers with no copy constructor, so a copy would double-free.

         Records are numbered 0 .. n_records-1 and the index is passed to the fill
         function so tests can vary content per record.

         Stream sequence: s_start, n_records x s_run, s_stop, s_exit.
        */
        template< class x_type >
        class test_producer : public _producer< type_list< x_type > >
        {
            public:
                typedef std::function< void ( x_type*, std::size_t ) > fill_func_t;

                test_producer() :
                        f_n_records( 1 ),
                        f_buffer_size( 10 ),
                        f_fill( []( x_type*, std::size_t ){ return; } ),
                        f_n_sent( 0 ),
                        f_prep( nullptr )
                {}
                virtual ~test_producer() {}

                mv_accessible( std::size_t, n_records );
                mv_accessible( std::size_t, buffer_size );

                /// Per-record, in-place fill; must not copy x_type
                void set_fill( fill_func_t a_fill ) { f_fill = a_fill; }

                /// Optional one-time preparation applied to every slot at initialize()
                /// (e.g. allocate_array). Runs before any record is sent.
                void set_prepare( fill_func_t a_prep ) { f_prep = a_prep; }

                /// Number of s_run records actually sent (thread-safe read after run())
                std::size_t n_sent() const { return f_n_sent.load(); }

            public:
                virtual void initialize()
                {
                    this->template out_buffer< 0 >().initialize( f_buffer_size );
                }

                virtual void execute( diptera* a_midge = nullptr )
                {
                    try
                    {
                        if( ! this->template out_stream< 0 >().set( stream::s_start ) ) return;

                        for( std::size_t t_i = 0; t_i < f_n_records; ++t_i )
                        {
                            if( this->is_canceled() ) break;

                            x_type* t_slot = this->template out_stream< 0 >().data();
                            if( f_prep ) f_prep( t_slot, t_i );
                            f_fill( t_slot, t_i );

                            if( this->template out_stream< 0 >().get() == stream::s_stop ) break;
                            if( ! this->template out_stream< 0 >().set( stream::s_run ) ) break;

                            ++f_n_sent;
                        }

                        this->template out_stream< 0 >().set( stream::s_stop );
                        this->template out_stream< 0 >().set( stream::s_exit );
                    }
                    catch( ... )
                    {
                        if( a_midge ) a_midge->throw_ex( std::current_exception() );
                    }
                }

                virtual void finalize()
                {
                    this->template out_buffer< 0 >().finalize();
                }

            private:
                fill_func_t f_fill;
                std::atomic< std::size_t > f_n_sent;
                fill_func_t f_prep;
        };


        /*!
         @class test_consumer
         @brief Consumer that invokes a caller-supplied extract function on each s_run
                record, counts records, and cancels the run once done.

         @details
         The extract function must deep-copy whatever the test cares about out of the
         record into caller-owned storage:
             void extract( const x_type* a_record, std::size_t a_index )
         Never store x_type itself -- payload types commonly hold raw owning pointers
         with no copy constructor.

         Because many midge nodes never break out of their execute() loop on s_stop or
         s_exit, this consumer drives shutdown: once it has seen expect_records records
         AND a terminal stream command (s_stop / s_exit / s_error), it calls
         diptera::cancel(), which cancels producers, then transformers, then consumers --
         the same path production uses.

         Set expect_records to 0 to shut down on the first terminal command regardless
         of count.
        */
        template< class x_type >
        class test_consumer : public _consumer< type_list< x_type > >
        {
            public:
                typedef std::function< void ( const x_type*, std::size_t ) > extract_func_t;

                test_consumer() :
                        f_expect_records( 0 ),
                        f_extract( []( const x_type*, std::size_t ){ return; } ),
                        f_n_received( 0 ),
                        f_n_starts( 0 ),
                        f_n_stops( 0 ),
                        f_saw_error( false )
                {}
                virtual ~test_consumer() {}

                mv_accessible( std::size_t, expect_records );

                void set_extract( extract_func_t a_extract ) { f_extract = a_extract; }

                std::size_t n_received() const { return f_n_received.load(); }
                std::size_t n_starts()   const { return f_n_starts.load(); }
                std::size_t n_stops()    const { return f_n_stops.load(); }
                bool saw_error()         const { return f_saw_error.load(); }

            public:
                virtual void initialize() {}

                virtual void execute( diptera* a_midge = nullptr )
                {
                    try
                    {
                        while( ! this->is_canceled() )
                        {
                            enum_t t_cmd = this->template in_stream< 0 >().get();

                            if( t_cmd == stream::s_none ) continue;

                            if( t_cmd == stream::s_start )
                            {
                                ++f_n_starts;
                                continue;
                            }

                            if( t_cmd == stream::s_run )
                            {
                                const x_type* t_rec = this->template in_stream< 0 >().data();
                                f_extract( t_rec, f_n_received.load() );
                                ++f_n_received;
                                continue;
                            }

                            if( t_cmd == stream::s_stop )
                            {
                                ++f_n_stops;
                                if( done() && a_midge ) { a_midge->cancel(); break; }
                                continue;
                            }

                            if( t_cmd == stream::s_exit )
                            {
                                if( a_midge ) a_midge->cancel();
                                break;
                            }

                            if( t_cmd == stream::s_error )
                            {
                                f_saw_error.store( true );
                                if( a_midge ) a_midge->cancel();
                                break;
                            }
                        }
                    }
                    catch( ... )
                    {
                        if( a_midge ) a_midge->throw_ex( std::current_exception() );
                    }
                }

                virtual void finalize() {}

            private:
                bool done() const
                {
                    return f_expect_records == 0 || f_n_received.load() >= f_expect_records;
                }

                extract_func_t f_extract;
                std::atomic< std::size_t > f_n_received;
                std::atomic< std::size_t > f_n_starts;
                std::atomic< std::size_t > f_n_stops;
                std::atomic< bool > f_saw_error;
        };
    }
}

#endif
