#ifndef _midge_rt_rt_sum_transformer_hh_
#define _midge_rt_rt_sum_transformer_hh_

#include "transformer.hh"
#include "rt_data.hh"
#include "window.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class rt_rt_sum_transformer :
        public _transformer< rt_rt_sum_transformer, typelist_2( rt_data, rt_data ), typelist_1( rt_data ) >
    {
        public:
            rt_rt_sum_transformer();
            virtual ~rt_rt_sum_transformer();

        public:
            accessible( count_t, length );

        protected:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
