#ifndef _midge_rf_cf_transformer_hh_
#define _midge_rf_cf_transformer_hh_

#include "transformer.hh"
#include "rf_data.hh"
#include "cf_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class rf_cf_transformer :
        public _transformer< rf_cf_transformer, typelist_1( rf_data ),typelist_1( cf_data ) >
    {
        public:
            rf_cf_transformer();
            virtual ~rf_cf_transformer();

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
