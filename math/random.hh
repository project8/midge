#ifndef _midge_random_hh_
#define _midge_random_hh_

#include "singleton.hh"
#include "mutex.hh"
#include "types.hh"

#include "gsl/gsl_rng.h"

namespace midge
{

    typedef gsl_rng random_t;

    class random :
        public singleton< random >
    {
        public:
            friend class singleton< random >;

        private:
            random();
            virtual ~random();

        public:
            random_t* allocate( const count_t& p_seed );
            void free( random_t* p_random );

            real_t uniform( random_t* p_random, const real_t& p_low, const real_t& p_high );
            real_t gaussian( random_t* p_random, const real_t& p_mu, const real_t& p_sigma );

        private:
            mutex f_mutex;
    };

}

#endif
