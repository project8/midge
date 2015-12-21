#ifndef _midge_serializer_hh_
#define _midge_serializer_hh_

#include <stack>

#include "processor.hh"
#include "value.hh"
using std::stack;

#include <fstream>
using std::ofstream;

namespace midge
{

    class serializer :
        public processor
    {
        public:
            serializer( const string& p_file );
            virtual ~serializer();

        public:
            void operator()( value* p_value );

        private:
            void dispatch( value* p_value );

        public:
            virtual void process_key( string p_string );
            virtual void process_lingual( string p_string );
            virtual void process_numerical( string p_string );
            virtual void process_boolean( string p_string );
            virtual void process_null();
            virtual void process_object_start();
            virtual void process_object_stop();
            virtual void process_array_start();
            virtual void process_array_stop();
            virtual void process_start();
            virtual void process_stop();

        private:
            string f_file;
            ofstream f_stream;

            class context
            {
                public:
                    context();
                    ~context();

                    string pad;
                    string key;
                    string comma;
            };

            stack< context > f_contexts;
    };
}

#endif
