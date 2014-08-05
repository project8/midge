#ifndef _midge_rf_root_consumer_hh_
#define _midge_rf_root_consumer_hh_

#include "consumer.hh"
#include "rf_data.hh"
#include "typelist.hh"
#include "macros.hh"

#include "TFile.h"
#include "TTree.h"

namespace midge
{

    class rf_root_consumer :
        public _consumer< rf_root_consumer, typelist_1( rf_data )>
    {
        public:
            rf_root_consumer();
            virtual ~rf_root_consumer();

        public:
            accessible( string, file )
            accessible( bool_t, plot )
            accessible( string, plot_key )
            accessible( string, plot_name )
            accessible( string, chart_title )
            accessible( string, axis_title )

        private:
            TFile* f_stream;
            TTree* f_tree;
            real_t f_time;
            real_t f_frequency;
            real_t f_value;

            count_t f_size;
            real_t f_interval;
            real_t* f_in;
            real_t f_first_time;
            real_t f_last_time;
            count_t f_count;

        protected:
            bool start_consumer();
            bool execute_consumer();
            bool stop_consumer();
    };

}

#endif
