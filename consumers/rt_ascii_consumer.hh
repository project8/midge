#ifndef _midge_rt_ascii_consumer_hh_
#define _midge_rt_ascii_consumer_hh_

#include "consumer.hh"
#include "rt_data.hh"
#include "typelist.hh"

#include <fstream>
using std::ofstream;

namespace midge
{

    class rt_ascii_consumer :
        public _consumer< rt_ascii_consumer, typelist_1( rt_data )>
    {
        public:
            rt_ascii_consumer();
            virtual ~rt_ascii_consumer();

        public:
            void set_file( const string& p_filename );
            const string& get_file() const;

        private:
            string f_file;

            ofstream f_stream;

            count_t f_size;
            real_t f_interval;
            real_t* f_in;

            real_t f_start_time;
            real_t f_stop_time;
            real_t f_current_time;

        protected:
            void initialize_consumer();
            bool start_consumer();
            bool execute_consumer();
            bool stop_consumer();
            void finalize_consumer();
    };

}

#endif
