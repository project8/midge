#ifndef _midge_ct_ctf_fourier_transformer_hh_
#define _midge_ct_ctf_fourier_transformer_hh_

#include "transformer.hh"
#include "ct_data.hh"
#include "ctf_data.hh"
#include "window.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class ct_ctf_fourier_transformer :
        public _transformer< ct_ctf_fourier_transformer, typelist_1( ct_data ), typelist_1( ctf_data ) >
    {
        public:
            ct_ctf_fourier_transformer();
            virtual ~ct_ctf_fourier_transformer();

        public:
            accessible( count_t, length );

        protected:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
