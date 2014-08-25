#ifndef _midge_input_hh_
#define _midge_input_hh_

#include "stream.hh"
#include "macros.hh"

namespace midge
{

    class node;

    class input
    {
        public:
            input();
            virtual ~input();

        public:
            accessible( string_t, name )

        public:
            virtual void set( stream* p_stream ) = 0;
    };

}

#endif
