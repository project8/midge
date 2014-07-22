#ifndef _midge_rt_ct_transformer_hh_
#define _midge_rt_ct_transformer_hh_

#include "transformer.hh"
#include "rt_data.hh"
#include "ct_data.hh"
#include "typelist.hh"

namespace midge
{

    class rt_ct_transformer :
        public _transformer< rt_ct_transformer, typelist_1( rt_data ),typelist_1( ct_data ) >
    {
        public:
            rt_ct_transformer();
            virtual ~rt_ct_transformer();

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
            real_t* f_in;
            complex_t* f_out;

        protected:
            void start_transformer();
            void execute_transformer();
            void stop_transformer();
    };
}

#endif
