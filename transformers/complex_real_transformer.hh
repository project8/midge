#ifndef _midge_complex_real_transformer_hh_
#define _midge_complex_real_transformer_hh_

#include "transformer.hh"
#include "real.hh"
#include "complex.hh"
#include "typelist.hh"

namespace midge
{

    class complex_real_transformer :
        public _transformer< complex_real_transformer, typelist_1( complex ),typelist_1( real ) >
    {
        public:
            complex_real_transformer();
            virtual ~complex_real_transformer();

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
            uint64_t f_size;
            complex_t* f_in_raw;
            real_t* f_out_raw;

        protected:
            void initialize_transformer();
            void execute_transformer();
    };
}

#endif
