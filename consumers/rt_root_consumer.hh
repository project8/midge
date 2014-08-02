#ifndef _midge_rt_root_consumer_hh_
#define _midge_rt_root_consumer_hh_

#include "consumer.hh"
#include "rt_data.hh"
#include "typelist.hh"

#include "TFile.h"
#include "TTree.h"

namespace midge
{

    class rt_root_consumer :
        public _consumer< rt_root_consumer, typelist_1( rt_data )>
    {
        public:
            rt_root_consumer();
            virtual ~rt_root_consumer();

        public:
            void set_file( const string& p_filename );
            const string& get_file() const;

            void set_plot( const bool_t& p_plot );
            const bool_t& get_plot() const;

        private:
            string f_file;
            bool_t f_plot;

            TFile* f_stream;
            TTree* f_tree;
            real_t f_time_point;
            real_t f_value_point;

            count_t f_size;
            real_t f_interval;
            real_t* f_in;
            real_t f_time;

        protected:
            void initialize_consumer();
            bool start_consumer();
            bool execute_consumer();
            bool stop_consumer();
            void finalize_consumer();
    };

}

#endif
