#ifndef _midge_real_ascii_consumer_hh_
#define _midge_real_ascii_consumer_hh_

#include "consumer.hh"
#include "real.hh"
#include "typelist.hh"

#include <fstream>
using std::ofstream;

namespace midge
{

    class real_ascii_consumer :
        public _consumer< real_ascii_consumer, typelist_1( real )>
    {
        public:
            real_ascii_consumer();
            virtual ~real_ascii_consumer();

        public:
            void set_file( const string& p_filename );
            const string& get_file() const;

        private:
            string f_file;
            ofstream f_stream;
            uint64_t f_internal;
            real_t* f_raw;
            uint64_t f_size;

        protected:
            void initialize_consumer();
            void execute_consumer();
            void finalize_consumer();
    };

}

#endif
