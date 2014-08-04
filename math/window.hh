#ifndef _midge_window_hh_
#define _midge_window_hh_

#include "types.hh"

namespace midge
{

    class window
    {
        public:
            window();
            virtual ~window();

        public:
            void initialize( const count_t& p_size );
            void finalize();

            const real_t* raw() const;
            const real_t& sum() const;
            const real_t& square() const;

        protected:
            virtual void initialize_window( const count_t& p_size, real_t*& p_raw ) = 0;
            virtual void finalize_window( real_t*& p_raw ) = 0;

        private:
            real_t* f_raw;
            real_t f_sum;
            real_t f_square;
    };
}

#endif
