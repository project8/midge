/*
 * test_harness_smoke.cc
 *
 * Smoke test for the midge testing harness (midge_test_nodes.hh +
 * midge_test_harness.hh). Uses a trivial int_data type so there is no
 * dependency on any project outside midge itself.
 *
 * Verifies:
 *   - test_producer emits exactly n_records s_run commands
 *   - test_consumer receives them and calls cancel when done
 *   - run_with_watchdog returns without timeout and without exception
 *   - captured values match what was written
 */

#include "midge_test_harness.hh"
#include "midge_test_nodes.hh"

#include <cassert>
#include <iostream>
#include <vector>

namespace
{
    class int_data
    {
        public:
            int_data() : f_value( 0 ) {}
            ~int_data() {}

            void set_value( int v ) { f_value = v; }
            int  get_value() const  { return f_value; }

        private:
            int f_value;
    };
}

int main()
{
    const std::size_t N = 5;
    std::vector< int > t_received;

    auto* t_prod = new midge::testing::test_producer< int_data >();
    auto* t_cons = new midge::testing::test_consumer< int_data >();

    t_prod->set_name( "prod" );
    t_cons->set_name( "cons" );

    t_prod->set_n_records( N );
    t_prod->set_buffer_size( 8 );
    t_prod->set_fill( []( int_data* a_slot, std::size_t a_i )
        { a_slot->set_value( static_cast< int >( a_i ) ); } );

    t_cons->set_expect_records( N );
    t_cons->set_extract( [&]( const int_data* a_rec, std::size_t )
        { t_received.push_back( a_rec->get_value() ); } );

    midge::diptera t_midge;
    t_midge.add( t_prod );
    t_midge.add( t_cons );
    t_midge.join( "prod.out_0:cons.in_0" );

    auto t_result = midge::testing::run_with_watchdog( t_midge, "prod:cons" );

    if( t_result.f_timed_out )
    {
        std::cerr << "FAIL: pipeline timed out\n";
        return 1;
    }
    if( t_result.f_exception )
    {
        std::cerr << "FAIL: pipeline threw an exception\n";
        return 1;
    }
    if( t_received.size() != N )
    {
        std::cerr << "FAIL: expected " << N << " records, got " << t_received.size() << "\n";
        return 1;
    }
    for( std::size_t i = 0; i < N; ++i )
    {
        if( t_received[i] != static_cast< int >( i ) )
        {
            std::cerr << "FAIL: record " << i << " has value " << t_received[i]
                      << ", expected " << i << "\n";
            return 1;
        }
    }

    std::cout << "PASS: harness smoke test\n";
    return 0;
}
