#ifndef _midge_window_hamming_hh_
#define _midge_window_hamming_hh_

#include "window.hh"

namespace midge
{

    class window_hamming :
        public window
    {
        public:
            window_hamming();
            virtual ~window_hamming();

        protected:
            void fill( const count_t& p_size, real_t*& p_raw );
    };
}

#endif
