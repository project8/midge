#ifndef _midge_compiler_hh_
#define _midge_compiler_hh_

#include <stack>

#include "processor.hh"
#include "value.hh"

namespace midge
{

    class compiler :
        public processor
    {
        public:
            compiler();
            virtual ~compiler();

        public:
            value* operator()();

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
            void add_single( value* p_value );
            void add_composite( value* p_value );

            value* f_result;

            std::stack< value* > f_values;
            std::string f_key;
    };
}

#endif
