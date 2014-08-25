#ifndef _midge_rt_rtf_wigner_transformer_hh_
#define _midge_rt_rtf_wigner_transformer_hh_

#include "transformer.hh"
#include "rt_data.hh"
#include "rtf_data.hh"
#include "window.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class rt_rtf_wigner_transformer :
        public _transformer< rt_rtf_wigner_transformer, typelist_1( rt_data ), typelist_1( rtf_data ) >
    {
        public:
            rt_rtf_wigner_transformer();
            virtual ~rt_rtf_wigner_transformer();

        public:
            composable( window, window );
            accessible( count_t, width );
            accessible( count_t, length );

        protected:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
