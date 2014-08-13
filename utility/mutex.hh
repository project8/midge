#ifndef mantis_mutex_hh_
#define mantis_mutex_hh_

#include <pthread.h>

#include <cstddef>

namespace midge
{

    class mutex
    {
        public:
            mutex();
            virtual ~mutex();

            bool trylock();

            void lock();
            void unlock();

        private:
            pthread_mutex_t f_mutex;
    };

}

#endif
