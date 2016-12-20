#ifndef _midge_reporter_hh_
#define _midge_reporter_hh_

#include <stack>

#include "processor.hh"
#include "value.hh"

namespace midge
{

    class reporter :
        public processor
    {
        public:
            reporter();
            virtual ~reporter();

        public:
            void operator()( value* f_value );

        private:
            void dispatch( value* f_value );

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

        private:
            class context
            {
                public:
                    context();
                    ~context();

                    std::string pad;
                    std::string key;
                    std::string comma;
            };

            std::stack< context > f_contexts;
    };
}

#endif
