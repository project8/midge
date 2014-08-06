#ifndef _midge_rf_rf_threshold_transformer_hh_
#define _midge_rf_rf_threshold_transformer_hh_

#include "transformer.hh"
#include "rf_data.hh"
#include "typelist.hh"
#include "macros.hh"

#include "TFile.h"
#include "TObjString.h"
#include "TTree.h"

#include <list>
using std::list;

namespace midge
{

    class rf_rf_threshold_transformer :
        public _transformer< rf_rf_threshold_transformer, typelist_1( rf_data ), typelist_1( rf_data ) >
    {
        public:
            rf_rf_threshold_transformer();
            ~rf_rf_threshold_transformer();

        public:
            accessible( string, threshold_file )
            accessible( string, background_file )
            accessible( real_t, threshold )
            accessible( real_t, minimum_frequency )
            accessible( real_t, maximum_frequency )
            accessible( bool_t, plot )
            accessible( string, plot_key )
            accessible( string, plot_name )
            accessible( string, chart_title )
            accessible( string, axis_title )

        private:
            TFile* f_stream;
            TObjString* f_label;
            TTree* f_tree;
            real_t f_time;
            real_t f_frequency;
            real_t f_value;

            count_t f_size;
            real_t f_interval;
            const real_t* f_in;
            real_t* f_out;

            real_t* f_background;
            count_t f_minimum_frequency_index;
            count_t f_maximum_frequency_index;
            real_t f_first_time;
            real_t f_last_time;
            count_t f_count;

        protected:
            bool start_transformer();
            bool execute_transformer();
            bool stop_transformer();
    };

}

#endif
