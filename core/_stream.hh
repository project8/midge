#ifndef _midge__stream_hh_
#define _midge__stream_hh_

#include "stream.hh"

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
            virtual _stream< x_type >& operator>>( x_type& p_data ) = 0;
            virtual _stream< x_type >& operator<<( const x_type& p_data ) = 0;
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
