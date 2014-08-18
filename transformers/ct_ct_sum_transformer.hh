#ifndef _midge_ct_ct_sum_transformer_hh_
#define _midge_ct_ct_sum_transformer_hh_

#include "transformer.hh"
#include "ct_data.hh"
#include "window.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class ct_ct_sum_transformer :
        public _transformer< ct_ct_sum_transformer, typelist_2( ct_data, ct_data ), typelist_1( ct_data ) >
    {
        public:
            ct_ct_sum_transformer();
            virtual ~ct_ct_sum_transformer();

        public:
            accessible( count_t, length );

        protected:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
