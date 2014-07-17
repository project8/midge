#ifndef _midge_ascii_real_consumer_hh_
#define _midge_ascii_real_consumer_hh_

#include "consumer.hh"
#include "real.hh"
#include "typelist.hh"

#include <fstream>
using std::ofstream;

namespace midge
{

    class ascii_real_consumer :
        public consumer< ascii_real_consumer, typelist_1( real )>
    {
        public:
            ascii_real_consumer();
            virtual ~ascii_real_consumer();

        public:
            void set_filename( const string& p_filename );
            const string& get_filename() const;

        private:
            string f_filename;
            ofstream f_filestream;
            uint64_t f_internal;

        protected:
            void initialize_consumer();
            void execute_consumer();
            void finalize_consumer();
    };

}

#endif
