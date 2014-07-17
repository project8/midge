#ifndef _midge_processor_terminal_hh_
#define _midge_processor_terminal_hh_

#include "processor.hh"

namespace midge
{

    class processor_terminal :
        public processor
    {
        public:
            processor_terminal();
            virtual ~processor_terminal();

        public:
            virtual void process_key( token* p_token );
            virtual void process_string( token* p_token );
            virtual void process_boolean( token* p_token );
            virtual void process_numerical( token* p_token );
            virtual void process_object_start();
            virtual void process_object_stop();
            virtual void process_array_start();
            virtual void process_array_stop();
            virtual void process_start();
            virtual void process_stop();
    };
}

#endif
