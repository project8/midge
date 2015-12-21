#ifndef _midge__stream_hh_
#define _midge__stream_hh_

#include "../library/core/stream.hh"

namespace midge
{

    template< class x_type >
    class _stream :
        public stream
    {

        protected:
            _stream();
            virtual ~_stream();

        public:
            virtual x_type* data() = 0;
    };

    template< class x_type >
    _stream< x_type >::_stream()
    {
    }

    template< class x_type >
    _stream< x_type >::~_stream()
    {
    }

}

#endif
