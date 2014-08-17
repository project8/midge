#ifndef _midge_cf_rf_transformer_hh_
#define _midge_cf_rf_transformer_hh_

#include "transformer.hh"
#include "rf_data.hh"
#include "cf_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class cf_rf_transformer :
        public _transformer< cf_rf_transformer, typelist_1( cf_data ),typelist_1( rf_data ) >
    {
        public:
            cf_rf_transformer();
            virtual ~cf_rf_transformer();

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
