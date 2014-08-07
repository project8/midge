#ifndef _midge_window_blackman_hh_
#define _midge_window_blackman_hh_

#include "window.hh"

namespace midge
{

    class window_blackman :
        public window
    {
        public:
            window_blackman();
            virtual ~window_blackman();

        protected:
            void fill( const count_t& p_size, real_t*& p_raw );
    };
}

#endif
