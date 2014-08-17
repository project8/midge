#ifndef _midge_rt_rf_power_transformer_hh_
#define _midge_rt_rf_power_transformer_hh_

#define BUFFER_DEBUG 1

#include "transformer.hh"
#include "rt_data.hh"
#include "rf_data.hh"
#include "window.hh"
#include "typelist.hh"
#include "macros.hh"

#undef BUFFER_DEBUG

namespace midge
{

    class rt_rf_power_transformer :
        public _transformer< rt_rf_power_transformer, typelist_1( rt_data ), typelist_1( rf_data ) >
    {
        public:
            rt_rf_power_transformer();
            virtual ~rt_rf_power_transformer();

        public:
            accessible( real_t, impedance_ohm );
            composable( window, window );
            accessible( count_t, length );

        protected:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
