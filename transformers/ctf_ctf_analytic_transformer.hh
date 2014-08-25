#ifndef _midge_ctf_ctf_analytic_transformer_hh_
#define _midge_ctf_ctf_analytic_transformer_hh_

#include "transformer.hh"
#include "ctf_data.hh"
#include "window.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class ctf_ctf_analytic_transformer :
        public _transformer< ctf_ctf_analytic_transformer, typelist_1( ctf_data ), typelist_1( ctf_data ) >
    {
        public:
            ctf_ctf_analytic_transformer();
            virtual ~ctf_ctf_analytic_transformer();

        public:
            accessible( count_t, length );

        protected:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
