#ifndef _midge_input_hh_
#define _midge_input_hh_

#include "stream.hh"
#include "member_variables.hh"

namespace midge
{

    class node;

    class input
    {
        public:
            input();
            virtual ~input();

        public:
            mv_accessible( string_t, name )

        public:
            virtual void set( stream* p_stream ) = 0;
    };

}

#endif
