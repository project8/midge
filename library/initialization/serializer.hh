#ifndef _midge_serializer_hh_
#define _midge_serializer_hh_

#include <stack>

#include "processor.hh"
#include "value.hh"

#include <fstream>

namespace midge
{

    class serializer :
        public processor
    {
        public:
            serializer( const std::string& p_file );
            virtual ~serializer();

        public:
            void operator()( value* p_value );

        private:
            void dispatch( value* p_value );

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
            std::string f_file;
            std::ofstream f_stream;

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
