#ifndef _midge_stream_hh_
#define _midge_stream_hh_

#include "types.hh"

namespace midge
{

    class stream
    {
        public:
            static const command_t s_none;
            static const command_t s_start;
            static const command_t s_run;
            static const command_t s_stop;
            static const command_t s_exit;

        protected:
            stream();
            virtual ~stream();

        public:
            virtual command_t command() = 0;
            virtual void command( command_t p_command ) = 0;
    };

}

#endif
