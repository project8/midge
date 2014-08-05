#ifndef _midge_rf_average_root_consumer_hh_
#define _midge_rf_average_root_consumer_hh_

#include "consumer.hh"
#include "rf_data.hh"
#include "typelist.hh"
#include "macros.hh"

#include "TFile.h"
#include "TTree.h"

namespace midge
{

    class rf_average_root_consumer :
        public _consumer< rf_average_root_consumer, typelist_1( rf_data )>
    {
        public:
            rf_average_root_consumer();
            virtual ~rf_average_root_consumer();

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
            real_t f_frequency_point;
            real_t f_value_point;

            count_t f_size;
            real_t f_interval;
            real_t* f_in;
            count_t f_count;
            real_t* f_average;

        protected:
            bool start_consumer();
            bool execute_consumer();
            bool stop_consumer();
    };

}

#endif
