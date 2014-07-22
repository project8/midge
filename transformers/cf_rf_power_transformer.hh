#ifndef _midge_cf_rf_power_transformer_hh_
#define _midge_cf_rf_power_transformer_hh_

#include "transformer.hh"
#include "cf_data.hh"
#include "rf_data.hh"
#include "typelist.hh"

namespace midge
{

    class cf_rf_power_transformer :
        public _transformer< cf_rf_power_transformer, typelist_1( cf_data ), typelist_1( rf_data ) >
    {
        public:
            cf_rf_power_transformer();
            virtual ~cf_rf_power_transformer();

        public:
            void set_impedance_ohm( const real_t& p_impedance );
            const real_t& get_impedance_ohm() const;

        private:
            real_t f_impedance_ohm;

            count_t f_last;
            bool_t f_nyquist;
            complex_t* f_in;
            real_t* f_out;
            real_t f_norm;

        protected:
            void start_transformer();
            void execute_transformer();
            void stop_transformer();
    };

}

#endif
