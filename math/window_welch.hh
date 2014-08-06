#ifndef _midge_window_welch_hh_
#define _midge_window_welch_hh_

#include "window.hh"

namespace midge
{

    class window_welch :
        public window
    {
        public:
            window_welch();
            virtual ~window_welch();

        protected:
            void fill( const count_t& p_size, real_t*& p_raw );
    };
}

#endif
