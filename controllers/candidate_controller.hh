#ifndef _midge_candidate_controller_hh_
#define _midge_candidate_controller_hh_

#include "controller.hh"
#include "rt_data.hh"
#include "rf_data.hh"
#include "rt_monarch_producer.hh"
#include "typelist.hh"
#include "macros.hh"

#include "TFile.h"
#include "TTree.h"

namespace midge
{

    class candidate_controller :
        public _controller< candidate_controller, typelist_2( rt_data, rf_data ), typelist_1( rt_monarch_producer ) >
    {
        public:
            candidate_controller();
            ~candidate_controller();

        public:
            accessible( string, candidate_file )
            accessible( string, background_file )
            accessible( string, background_name )
            accessible( count_t, rough_stride )
            accessible( count_t, fine_stride )
            accessible( real_t, threshold_ratio )
            accessible( real_t, minimum_time )
            accessible( real_t, maximum_time )
            accessible( real_t, minimum_frequency )
            accessible( real_t, maximum_frequency )
            accessible( real_t, minimum_slope )
            accessible( real_t, maximum_slope )
            accessible( bool_t, plot )
            accessible( string, plot_title )
            accessible( string, axis_title )

        private:
            count_t f_signal_size;
            real_t f_signal_interval;
            real_t* f_signal;
            count_t f_power_frequency_size;
            real_t f_power_frequency_interval;
            real_t* f_power;
            real_t* f_background;

            count_t f_minimum_time_index;
            count_t f_maximum_time_index;
            count_t f_minimum_frequency_index;
            count_t f_maximum_frequency_index;
            real_t f_minimum_slope_value;
            real_t f_maximum_slope_value;

            TFile* f_stream;
            TTree* f_tree;
            real_t f_time;
            real_t f_frequency;
            real_t f_value;

        protected:
            //void initialize_controller();
            bool start_controller_pre();
            bool start_controller_post();
            bool execute_controller_pre();
            bool execute_controller_post();
            bool stop_controller_pre();
            bool stop_controller_post();
            //void finalize_controller();
    };

}

#endif
