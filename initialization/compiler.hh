#ifndef _midge_compiler_hh_
#define _midge_compiler_hh_

#include "processor.hh"
#include "value.hh"

#include <stack>
using std::stack;

namespace midge
{

    class compiler :
        public processor
    {
        public:
            compiler();
            virtual ~compiler();

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
            void add_single( value* p_value );
            void add_composite( value* p_value );

            value* f_result;

            stack< value* > f_values;
            string f_key;
    };
}

#endif
