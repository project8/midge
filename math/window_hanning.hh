#ifndef _midge_window_hanning_hh_
#define _midge_window_hanning_hh_

#include "window.hh"

namespace midge
{

    class window_hanning :
        public window
    {
        public:
            window_hanning();
            virtual ~window_hanning();

        protected:
            void fill( const count_t& p_size, real_t*& p_raw );
    };
}

#endif
