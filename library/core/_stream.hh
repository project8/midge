#ifndef _midge__stream_hh_
#define _midge__stream_hh_

#include "stream.hh"

#include "macros.hh"
#include "stream_timer.hh"


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
            stream_timer f_timer;

    };

    template< class x_type >
    _stream< x_type >::_stream() :
            stream(),
            f_timer()
    {
    }

    template< class x_type >
    _stream< x_type >::~_stream()
    {
    }

    template< class x_type >
    inline void _stream< x_type >::timer_report() const
    {
        f_timer.report( f_label );
        return;
    }

}

#endif
