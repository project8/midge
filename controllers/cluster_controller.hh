#ifndef _midge_cluster_controller_hh_
#define _midge_cluster_controller_hh_

#include "controller.hh"
#include "rt_data.hh"
#include "rf_data.hh"
#include "rt_monarch_producer.hh"
#include "typelist.hh"
#include "macros.hh"

#include "TFile.h"
#include "TTree.h"

#include <list>
using std::list;

namespace midge
{

    class cluster_controller :
        public _controller< cluster_controller, typelist_2( rt_data, rf_data ), typelist_1( rt_monarch_producer ) >
    {
        public:
            cluster_controller();
            ~cluster_controller();

        public:
            accessible( string, threshold_file )
            accessible( string, background_file )
            accessible( string, background_name )
            accessible( real_t, minimum_frequency )
            accessible( real_t, maximum_frequency )
            accessible( real_t, threshold )
            accessible( bool_t, plot )
            accessible( string, plot_title )
            accessible( string, axis_title )

        private:
            count_t f_signal_size;
            real_t f_signal_interval;
            real_t* f_signal;
            count_t f_frequency_size;
            real_t f_frequency_interval;
            real_t* f_power;
            real_t* f_background;
            real_t* f_values;

            count_t f_minimum_frequency_index;
            count_t f_maximum_frequency_index;

            TFile* f_stream;

            TTree* f_threshold_tree;
            real_t f_time;
            real_t f_frequency;
            real_t f_value;

            real_t f_start_time;
            real_t f_stop_time;
            real_t f_start_frequency;
            real_t f_stop_frequency;

        private:
            class cluster
            {
                public:
                    static void set_time( const real_t& p_time );
                    static void set_size( const count_t& p_size );
                    static void set_value( const real_t* p_value );

                private:
                    static real_t f_time;
                    static count_t f_size;
                    static real_t* f_value;

                public:
                    cluster( const count_t& p_index );
                    ~cluster();

                public:
                    void update();

                    const real_t& get_slope();
                    const real_t& get_intercept();

                private:
                    count_t f_updates;

                    real_t f_x_sum;
                    real_t f_y_sum;
                    real_t f_xx_sum;
                    real_t f_yy_sum;
                    real_t f_xy_sum;

                    real_t f_slope;
                    real_t f_intercept;
                    real_t f_x_variance;
                    real_t f_y_variance;
                    real_t f_xy_covariance;

                    vector< real_t > f_t_points;
                    vector< real_t > f_f_points;
            };

            list< cluster* > f_clusters;

        protected:
            bool start_controller_pre();
            bool start_controller_post();
            bool execute_controller_pre();
            bool execute_controller_post();
            bool stop_controller_pre();
            bool stop_controller_post();
    };

}

#endif
