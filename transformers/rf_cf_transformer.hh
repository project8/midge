#ifndef _midge_rf_cf_transformer_hh_
#define _midge_rf_cf_transformer_hh_

#include "transformer.hh"
#include "rf_data.hh"
#include "cf_data.hh"
#include "typelist.hh"

namespace midge
{

    class rf_cf_transformer :
        public _transformer< rf_cf_transformer, typelist_1( rf_data ),typelist_1( cf_data ) >
    {
        public:
            rf_cf_transformer();
            virtual ~rf_cf_transformer();

        public:
            typedef enum
            {
                e_real = 0,
                e_imaginary = 1
            } mode;

            void set_mode( const string& p_string );
            const mode& get_mode() const;

        private:
            mode f_mode;

            count_t f_size;
            const real_t* f_in;
            complex_t* f_out;

        protected:
            bool start_transformer();
            bool execute_transformer();
            bool stop_transformer();
    };
}

#endif
