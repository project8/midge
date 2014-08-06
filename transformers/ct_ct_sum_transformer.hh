#ifndef _midge_ct_ct_sum_transformer_hh_
#define _midge_ct_ct_sum_transformer_hh_

#include "transformer.hh"
#include "ct_data.hh"
#include "typelist.hh"

namespace midge
{

    class ct_ct_sum_transformer :
        public _transformer< ct_ct_sum_transformer, typelist_2( ct_data, ct_data ),typelist_1( ct_data ) >
    {
        public:
            ct_ct_sum_transformer();
            virtual ~ct_ct_sum_transformer();

        private:
            count_t f_size;
            const complex_t* f_in_0;
            const complex_t* f_in_1;
            complex_t* f_out;

        protected:
            bool start_transformer();
            bool execute_transformer();
            bool stop_transformer();
    };
}

#endif
