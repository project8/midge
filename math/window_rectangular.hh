#ifndef _midge_window_rectangular_hh_
#define _midge_window_rectangular_hh_

#include "window.hh"

namespace midge
{

    class window_rectangular :
        public window
    {
        public:
            window_rectangular();
            virtual ~window_rectangular();

        protected:
            void fill( const count_t& p_size, real_t*& p_raw );
    };
}

#endif
