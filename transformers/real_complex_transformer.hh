#ifndef _midge_real_complex_transformer_hh_
#define _midge_real_complex_transformer_hh_

#include "transformer.hh"
#include "real.hh"
#include "complex.hh"
#include "typelist.hh"

namespace midge
{

    class real_complex_transformer :
        public transformer< real_complex_transformer, typelist_1( real ),typelist_1( complex ) >
    {
        public:
            real_complex_transformer();
            virtual ~real_complex_transformer();

        public:
            typedef enum
            {
                e_real = 0,
                e_imaginary = 1
            } mode;

            void set_mode( const mode& p_mode );
            const mode& get_mode() const;

        private:
            mode f_mode;
            uint64_t f_size;
            real_t* f_input;
            complex_t* f_output;

        protected:
            void initialize_transformer();
            void execute_transformer();
    };
}

#endif
