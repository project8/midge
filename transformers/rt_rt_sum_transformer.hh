#ifndef _midge_rt_rt_sum_transformer_hh_
#define _midge_rt_rt_sum_transformer_hh_

#include "transformer.hh"
#include "rt_data.hh"
#include "typelist.hh"

namespace midge
{

    class rt_rt_sum_transformer :
        public _transformer< rt_rt_sum_transformer, typelist_2( rt_data, rt_data ),typelist_1( rt_data ) >
    {
        public:
            rt_rt_sum_transformer();
            virtual ~rt_rt_sum_transformer();

        private:
            count_t f_size;
            const real_t* f_in_0;
            const real_t* f_in_1;
            real_t* f_out;

        protected:
            bool start_transformer();
            bool execute_transformer();
            bool stop_transformer();
    };
}

#endif
