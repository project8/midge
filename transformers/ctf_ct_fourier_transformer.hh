#ifndef _midge_ctf_ct_fourier_transformer_hh_
#define _midge_ctf_ct_fourier_transformer_hh_

#include "transformer.hh"
#include "ct_data.hh"
#include "ctf_data.hh"
#include "window.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class ctf_ct_fourier_transformer :
        public _transformer< ctf_ct_fourier_transformer, typelist_1( ctf_data ), typelist_1( ct_data ) >
    {
        public:
            ctf_ct_fourier_transformer();
            virtual ~ctf_ct_fourier_transformer();

        public:
            accessible( count_t, length );

        protected:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
