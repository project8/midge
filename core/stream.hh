#ifndef _midge_stream_hh_
#define _midge_stream_hh_

#include "types.hh"

namespace midge
{

    class stream
    {
        public:
            static const enum_t s_none;
            static const enum_t s_start;
            static const enum_t s_run;
            static const enum_t s_stop;
            static const enum_t s_exit;

        protected:
            stream();
            virtual ~stream();

        public:
            virtual enum_t get() = 0;
            virtual void set( enum_t p_command ) = 0;
    };

}

#endif
