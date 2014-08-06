#ifndef _midge_rf_candidate_consumer_hh_
#define _midge_rf_candidate_consumer_hh_

#include "consumer.hh"
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

    class rf_candidate_consumer :
        public _consumer< rf_candidate_consumer, typelist_1( rf_data ) >
    {
        public:
            rf_candidate_consumer();
            ~rf_candidate_consumer();

        public:
            accessible( string, threshold_file )
            accessible( string, cluster_file )
            accessible( string, background_file )
            accessible( real_t, frequency_minimum )
            accessible( real_t, frequency_maximum )
            accessible( real_t, threshold )
            accessible( real_t, cluster_slope )
            accessible( real_t, cluster_spread )
            accessible( real_t, cluster_add_coefficient )
            accessible( real_t, cluster_add_power )
            accessible( real_t, cluster_gap_coefficient )
            accessible( real_t, cluster_gap_power )
            accessible( real_t, cluster_score_up )
            accessible( real_t, cluster_score_down )
            accessible( bool_t, plot )
            accessible( string, plot_threshold_key )
            accessible( string, plot_threshold_name )
            accessible( string, chart_threshold_title )
            accessible( string, axis_threshold_title )
            accessible( string, plot_cluster_key )
            accessible( string, plot_cluster_name )
            accessible( string, chart_cluster_title )
            accessible( string, axis_cluster_title )

        private:
            TFile* f_threshold_stream;
            TObjString* f_threshold_label;
            TTree* f_threshold_tree;

            TFile* f_cluster_stream;
            TObjString* f_cluster_label;
            TTree* f_cluster_tree;

            real_t f_tree_time;
            real_t f_tree_frequency;
            real_t f_tree_value;
            count_t f_tree_id;
            real_t f_tree_score;

            count_t f_size;
            real_t f_interval;
            const real_t* f_in;

            real_t* f_signal;
            real_t* f_background;
            count_t f_frequency_minimum_index;
            count_t f_frequency_maximum_index;
            real_t f_minimum_time;
            real_t f_current_time;
            count_t f_count;

        protected:
            bool start_consumer();
            bool execute_consumer();
            bool stop_consumer();

        private:
            class cluster
            {
                public:
                    static void set_time( real_t* p_time );
                    static void set_signal( real_t* p_signal );
                    static void set_interval( const real_t& p_interval );
                    static void set_min_index( const count_t& p_size );
                    static void set_max_index( const count_t& p_size );
                    static void set_slope( const real_t& p_slope );
                    static void set_spread( const real_t& p_spread );
                    static void set_add_coefficient( const real_t& p_add_coefficient );
                    static void set_add_power( const real_t& p_add_power );
                    static void set_gap_coefficient( const real_t& p_gap_coefficient );
                    static void set_gap_power( const real_t& p_gap_power );
                    static void set_id( const count_t& p_id );

                private:
                    static real_t* s_time;
                    static real_t* s_signal;
                    static count_t s_min_index;
                    static count_t s_max_index;
                    static real_t s_interval;
                    static real_t s_slope;
                    static real_t s_spread;
                    static real_t s_add_coefficient;
                    static real_t s_add_power;
                    static real_t s_gap_coefficient;
                    static real_t s_gap_power;
                    static count_t s_id;

                public:
                    cluster( const count_t& p_index );
                    ~cluster();

                public:
                    void update();

                    const count_t& id();
                    const real_t& score();

                    const vector< real_t >& times() const;
                    const vector< real_t >& frequencies() const;
                    const vector< real_t >& values() const;

                private:
                    real_t compute_add_score();
                    real_t compute_frequency();

                    real_t f_time;
                    real_t f_frequency;
                    real_t f_numerator_sum;
                    real_t f_denominator_sum;
                    real_t f_add_score_sum;
                    real_t f_gap_score_sum;
                    real_t f_gap_score;
                    real_t f_gap_count;

                    count_t f_id;
                    real_t f_score;

                    vector< real_t > f_times;
                    vector< real_t > f_frequencies;
                    vector< real_t > f_values;
            };

            typedef list< cluster* > cluster_list;
            typedef cluster_list::iterator cluster_it;
            typedef cluster_list::const_iterator cluster_cit;

            cluster_list f_active_clusters;
            cluster_list f_completed_clusters;
    };

}

#endif
