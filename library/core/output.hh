#ifndef midge_output_hh_
#define midge_output_hh_

#include "stream.hh"
#include "member_variables.hh"

namespace midge
{

    class node;

    class output
    {
        public:
            output();
            virtual ~output();

        public:
            mv_accessible( string_t, name )

        public:
            virtual stream* get() = 0;
    };

}

#endif
