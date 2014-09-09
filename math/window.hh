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
            void set_size( const count_t& p_size );
            const count_t& get_size() const;

            const real_t& at( const count_t& p_index ) const;
            const real_t& sum() const;
            const real_t& square() const;

        protected:
            virtual void fill( const count_t& p_size, real_t*& p_raw ) = 0;

        private:
            count_t f_size;
            real_t* f_raw;
            real_t f_sum;
            real_t f_square;
    };
}

#endif
