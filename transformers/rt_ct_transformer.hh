#ifndef _midge_rt_ct_transformer_hh_
#define _midge_rt_ct_transformer_hh_

#include "transformer.hh"
#include "rt_data.hh"
#include "ct_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class rt_ct_transformer :
        public _transformer< rt_ct_transformer, typelist_1( rt_data ),typelist_1( ct_data ) >
    {
        public:
            rt_ct_transformer();
            virtual ~rt_ct_transformer();

        public:
            accessible( string_t, mode )
            accessible( count_t, length );

        protected:
            void initialize();
            void execute();
            void finalize();

        private:
            void real( const real_t& p_real, complex_t& p_complex );
            void imaginary( const real_t& p_real, complex_t& p_complex );
    };
}

#endif
