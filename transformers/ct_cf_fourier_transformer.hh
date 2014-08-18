#ifndef _midge_ct_cf_fourier_transformer_hh_
#define _midge_ct_cf_fourier_transformer_hh_

#include "transformer.hh"
#include "ct_data.hh"
#include "cf_data.hh"
#include "window.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class ct_cf_fourier_transformer :
        public _transformer< ct_cf_fourier_transformer, typelist_1( ct_data ), typelist_1( cf_data ) >
    {
        public:
            ct_cf_fourier_transformer();
            virtual ~ct_cf_fourier_transformer();

        public:
            accessible( count_t, length );

        protected:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
