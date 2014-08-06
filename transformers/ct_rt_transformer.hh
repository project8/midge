#ifndef _midge_ct_rt_transformer_hh_
#define _midge_ct_rt_transformer_hh_

#include "transformer.hh"
#include "rt_data.hh"
#include "ct_data.hh"
#include "typelist.hh"

namespace midge
{

    class ct_rt_transformer :
        public _transformer< ct_rt_transformer, typelist_1( ct_data ),typelist_1( rt_data ) >
    {
        public:
            ct_rt_transformer();
            virtual ~ct_rt_transformer();

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
            const complex_t* f_in;
            real_t* f_out;

        protected:
            bool start_transformer();
            bool execute_transformer();
            bool stop_transformer();
    };
}

#endif
