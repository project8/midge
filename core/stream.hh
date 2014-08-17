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
            virtual count_t operator++( int ) const = 0;

            virtual void state( const state_t& p_state ) = 0;
            virtual const state_t& state() const = 0;
    };

}

#endif
