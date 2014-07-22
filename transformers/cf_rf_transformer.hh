#ifndef _midge_cf_rf_transformer_hh_
#define _midge_cf_rf_transformer_hh_

#include "transformer.hh"
#include "rf_data.hh"
#include "cf_data.hh"
#include "typelist.hh"

namespace midge
{

    class cf_rf_transformer :
        public _transformer< cf_rf_transformer, typelist_1( cf_data ),typelist_1( rf_data ) >
    {
        public:
            cf_rf_transformer();
            virtual ~cf_rf_transformer();

        public:
            typedef enum
            {
                e_real = 0,
                e_imaginary = 1,
                e_modulus = 2,
                e_argument = 3
            } mode;

            void set_mode( const string& p_string );
            const mode& get_mode() const;

        private:
            mode f_mode;

            count_t f_size;
            complex_t* f_in;
            real_t* f_out;

        protected:
            void start_transformer();
            void execute_transformer();
            void stop_transformer();
    };
}

#endif
