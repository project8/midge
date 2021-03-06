#ifndef _midge_stream_hh_
#define _midge_stream_hh_

#include "stream_timer.hh"
#include "types.hh"

#include "cancelable.hh"

namespace midge
{

    class stream : public scarab::cancelable
    {
        public:
            static const enum_t s_none;
            static const enum_t s_start;
            static const enum_t s_run;
            static const enum_t s_stop;
            static const enum_t s_exit;
            static const enum_t s_error;

        protected:
            stream();
            virtual ~stream();

        public:
            virtual enum_t get() = 0;
            virtual bool set( enum_t p_command ) = 0;
            virtual count_t get_current_index() const = 0;

            virtual void timer_report() const = 0;

    };

}

#endif
