#ifndef _midge_rtf_rtf_ratio_transformer_hh_
#define _midge_rtf_rtf_ratio_transformer_hh_

#include "transformer.hh"
#include "rtf_data.hh"
#include "window.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class rtf_rtf_ratio_transformer :
        public _transformer< rtf_rtf_ratio_transformer, typelist_1( rtf_data ), typelist_1( rtf_data ) >
    {
        public:
            rtf_rtf_ratio_transformer();
            ~rtf_rtf_ratio_transformer();

        public:
            accessible( string, file )
            accessible( count_t, length )

        protected:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
