#ifndef _midge_rf_background_consumer_hh_
#define _midge_rf_background_consumer_hh_

#include "consumer.hh"
#include "rf_data.hh"
#include "window.hh"
#include "typelist.hh"
#include "macros.hh"

#include "TFile.h"
#include "TObjString.h"
#include "TTree.h"

namespace midge
{

    class rf_background_consumer :
        public _consumer< rf_background_consumer, typelist_1( rf_data )>
    {
        public:
            rf_background_consumer();
            virtual ~rf_background_consumer();

        public:
            accessible( string, file )
            accessible( count_t, length )
            composable( window, window )
            accessible( bool_t, plot )
            accessible( string, plot_key )
            accessible( string, plot_name )
            accessible( string, chart_title )
            accessible( string, axis_title )

        private:
            TFile* f_stream;
            TObjString* f_label;
            TTree* f_tree;
            real_t f_frequency_point;
            real_t f_value_point;

            count_t f_size;
            real_t f_interval;
            const real_t* f_in;

            const real_t* f_multiplier;

            real_t* f_average;
            real_t* f_background;
            count_t f_count;

        protected:
            bool start_consumer();
            bool execute_consumer();
            bool stop_consumer();
    };

}

#endif
