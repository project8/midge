#ifndef _midge_stream_hh_
#define _midge_stream_hh_

#include "types.hh"

namespace midge
{

    class stream
    {
        public:
            static const state_t s_none;
            static const state_t s_start;
            static const state_t s_run;
            static const state_t s_stop;
            static const state_t s_exit;

        protected:
            stream();
            virtual ~stream();

        public:
            virtual const stream& operator>( state_t& p_state ) const = 0;
            virtual const stream& operator<( const state_t& p_state ) const = 0;
    };

}

#endif
