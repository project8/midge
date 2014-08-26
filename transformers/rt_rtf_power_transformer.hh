#ifndef _midge_rt_rtf_power_transformer_hh_
#define _midge_rt_rtf_power_transformer_hh_

#include "transformer.hh"
#include "rt_data.hh"
#include "rtf_data.hh"
#include "window.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class rt_rtf_power_transformer :
        public _transformer< rt_rtf_power_transformer, typelist_1( rt_data ), typelist_1( rtf_data ) >
    {
        public:
            rt_rtf_power_transformer();
            virtual ~rt_rtf_power_transformer();

        public:
            accessible( real_t, impedance_ohm )
            accessible( real_t, begin_hz )
            accessible( real_t, end_hz )
            assignable( window, window )
            accessible( count_t, length )

        protected:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
