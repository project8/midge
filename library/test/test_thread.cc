#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"

#include <cmath>
#include <unistd.h>
#include "../utility/message.hh"
#include "../utility/thread.hh"
#include "../utility/types.hh"

message_declare( sleepmsg )
message_define( sleepmsg, sleep, sleep )

namespace midge
{
    namespace test
    {
        class sleeper
        {
            public:
                sleeper( const count_t& p_seed ) :
                        f_seed( p_seed ),
                        f_rng( NULL )
                {
                    f_rng = gsl_rng_alloc( gsl_rng_mt19937 );
                    gsl_rng_set( f_rng, f_seed );
                }
                ~sleeper()
                {
                    gsl_rng_free( f_rng );
                }

                void start()
                {
                    count_t t_time;
                    while( true )
                    {
                        t_time = (useconds_t) (round( gsl_ran_flat( f_rng, 250000., 750000. ) ));
                        sleepmsg( s_normal ) << "<" << f_seed << "> says: yawn! just let me sleep for <" << t_time << "> more microseconds..." << eom;
                        usleep( t_time );
                    }

                    return;
                }
                void stop()
                {
                    sleepmsg( s_warning ) << "<" << f_seed << "> says: how rude! fine, fine, i'll get up..." << eom;
                    return;
                }

            private:
                count_t f_seed;
                gsl_rng* f_rng;

        };
    }
}

using namespace midge;
using namespace midge::test;

int main()
{
    sleeper t_sleeper_one( 51385 );
    thread t_thread_one;
    t_thread_one.start( &t_sleeper_one, &sleeper::start );
    t_thread_one.stop( &t_sleeper_one, &sleeper::stop );

    sleeper t_sleeper_two( 112383 );
    thread t_thread_two;
    t_thread_two.start( &t_sleeper_two, &sleeper::start );
    t_thread_two.stop( &t_sleeper_two, &sleeper::stop );

    sleeper t_sleeper_three( 31387 );
    thread t_thread_three;
    t_thread_three.start( &t_sleeper_three, &sleeper::start );
    t_thread_three.stop( &t_sleeper_three, &sleeper::stop );

    sleepmsg( s_normal ) << "<master> says: starting sleepers..." << eom;

    t_thread_one.start();
    t_thread_two.start();
    t_thread_three.start();

    sleepmsg << "<master> says: you guys have five seconds..." << eom;
    usleep( 5000000 );

    sleepmsg << "<master> says: get up!" << eom;
    t_thread_one.stop();
    t_thread_two.stop();
    t_thread_three.stop();

    usleep( 1000000 );

    return 0;
}

