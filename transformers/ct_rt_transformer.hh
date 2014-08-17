#ifndef _midge_ct_rt_transformer_hh_
#define _midge_ct_rt_transformer_hh_

#include "transformer.hh"
#include "rt_data.hh"
#include "ct_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class ct_rt_transformer :
        public _transformer< ct_rt_transformer, typelist_1( ct_data ),typelist_1( rt_data ) >
    {
        public:
            ct_rt_transformer();
            virtual ~ct_rt_transformer();

        public:
            accessible( string_t, mode )
            accessible( count_t, length );

        protected:
            void initialize();
            void execute();
            void finalize();

        private:
            void real( const complex_t& p_complex, real_t& p_real );
            void imaginary( const complex_t& p_complex, real_t& p_real );
            void modulus( const complex_t& p_complex, real_t& p_real );
            void argument( const complex_t& p_complex, real_t& p_real );
    };
}

#endif
