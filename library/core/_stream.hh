#ifndef _midge__stream_hh_
#define _midge__stream_hh_

#include "stream.hh"

#include "macros.hh"


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

            virtual void timer_report() const;

            referrable( std::string, label );

        protected:
            IF_STREAM_TIMING_ENABLED( stream_timer f_timer; )

    };

    template< class x_type >
    _stream< x_type >::_stream() :
            stream()
    {
    }

    template< class x_type >
    _stream< x_type >::~_stream()
    {
    }

    template< class x_type >
    inline void _stream< x_type >::timer_report() const
    {
            IF_STREAM_TIMING_ENABLED( f_timer.report( f_label ); )
        return;
    }

}

#endif
