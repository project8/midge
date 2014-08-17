#include "random.hh"

#include "gsl/gsl_randist.h"

namespace midge
{

    random::random() :
            f_mutex()
    {
    }
    random::~random()
    {
    }

    random_t* random::allocate( const count_t& p_seed )
    {
        gsl_rng* t_rng;
        f_mutex.lock();
        t_rng = gsl_rng_alloc( gsl_rng_mt19937 );
        gsl_rng_set( t_rng, p_seed );
        f_mutex.unlock();
        return t_rng;
    }
    void random::free( random_t* p_random )
    {
        f_mutex.lock();
        gsl_rng_free( p_random );
        f_mutex.unlock();
        return;
    }

    real_t random::uniform( random_t* p_random, const real_t& p_low, const real_t& p_high )
    {
        return (gsl_ran_flat( p_random, p_low, p_high ));
    }
    real_t random::gaussian( random_t* p_random, const real_t& p_mu, const real_t& p_sigma )
    {
        return (p_mu + gsl_ran_gaussian( p_random, p_sigma ));
    }

}
