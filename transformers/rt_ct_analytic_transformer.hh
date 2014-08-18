#ifndef _midge_rt_ct_analytic_transformer_hh_
#define _midge_rt_ct_analytic_transformer_hh_

#include "transformer.hh"
#include "rt_data.hh"
#include "ct_data.hh"
#include "window.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class rt_ct_analytic_transformer :
        public _transformer< rt_ct_analytic_transformer, typelist_1( rt_data ), typelist_1( ct_data ) >
    {
        public:
            rt_ct_analytic_transformer();
            virtual ~rt_ct_analytic_transformer();

        public:
            accessible( count_t, length );

        protected:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
