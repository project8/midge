#ifndef _midge_processor_hh_
#define _midge_processor_hh_

#include "midge_error.hh"

namespace midge
{

    class processor
    {
        public:
            processor();
            virtual ~processor();

            //*********
            //structure
            //*********

        public:
            static void connect( processor* p_parent, processor* p_child );
            static void disconnect( processor* p_parent, processor* p_child );

            void insert_before( processor* p_target );
            void insert_after( processor* p_target );
            void remove();

            processor* get_first_parent();
            processor* get_parent();

            processor* get_last_child();
            processor* get_child();

        protected:
            processor* f_parent;
            processor* f_child;

            //*********
            //callbacks
            //*********

        public:
            virtual void process_key( std::string p_string );
            virtual void process_lingual( std::string p_string );
            virtual void process_numerical( std::string p_string );
            virtual void process_boolean( std::string p_string );
            virtual void process_null();
            virtual void process_object_start();
            virtual void process_object_stop();
            virtual void process_array_start();
            virtual void process_array_stop();
            virtual void process_start();
            virtual void process_stop();
    };

}

#endif
