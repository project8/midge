#ifndef _midge_processor_file_hh_
#define _midge_processor_file_hh_

#include "processor.hh"

namespace midge
{

    class processor_file :
        public processor
    {
        public:
            processor_file();
            virtual ~processor_file();

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

        private:
            string f_pre;
            string f_post;
    };
}

#endif
