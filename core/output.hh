#ifndef midge_output_hh_
#define midge_output_hh_

#include "stream.hh"
#include "macros.hh"

namespace midge
{

    class node;

    class output
    {
        public:
            output();
            virtual ~output();

        public:
            accessible( string_t, name )

        public:
            virtual const stream* get() = 0;
    };

}

#endif
