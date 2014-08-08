#ifndef _midge_rf_event_consumer_hh_
#define _midge_rf_event_consumer_hh_

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

    class rf_event_consumer :
        public _consumer< rf_event_consumer, typelist_1( rf_data ) >
    {
        public:
            rf_event_consumer();
            ~rf_event_consumer();

        public:
            accessible( string, ratio_file )
            accessible( string, background_file )
            accessible( real_t, frequency_minimum )
            accessible( real_t, frequency_maximum )
            accessible( string, cluster_file )
            accessible( real_t, cluster_add_ratio )
            accessible( real_t, cluster_slope )
            accessible( real_t, cluster_spread )
            accessible( real_t, cluster_add_coefficient )
            accessible( real_t, cluster_add_power )
            accessible( real_t, cluster_gap_coefficient )
            accessible( real_t, cluster_gap_power )
            accessible( string, line_file )
            accessible( real_t, line_start_score )
            accessible( real_t, line_stop_score )
            accessible( real_t, line_tolerance )
            accessible( real_t, line_add_coefficient )
            accessible( real_t, line_add_power )
            accessible( real_t, line_gap_coefficient )
            accessible( real_t, line_gap_power )
            accessible( string, event_file )
            accessible( real_t, event_time_tolerance )
            accessible( bool_t, plot )
            accessible( string, plot_ratio_key )
            accessible( string, plot_ratio_name )
            accessible( string, chart_ratio_title )
            accessible( string, axis_ratio_title )
            accessible( string, plot_cluster_key )
            accessible( string, plot_cluster_name )
            accessible( string, chart_cluster_title )
            accessible( string, axis_cluster_title )
            accessible( string, plot_line_key )
            accessible( string, plot_line_name )
            accessible( string, chart_line_title )
            accessible( string, axis_line_title )
            accessible( string, plot_event_key )
            accessible( string, plot_event_name )
            accessible( string, chart_event_title )
            accessible( string, axis_event_title )

        private:
            TFile* f_ratio_stream;
            TObjString* f_ratio_label;
            TTree* f_ratio_tree;

            TFile* f_cluster_stream;
            TObjString* f_cluster_label;
            TTree* f_cluster_tree;

            TFile* f_line_stream;
            TObjString* f_line_label;
            TTree* f_line_tree;

            TFile* f_event_stream;
            TObjString* f_event_label;
            TTree* f_event_tree;

            real_t f_tree_data_time;
            real_t f_tree_data_frequency;
            real_t f_tree_data_ratio;
            count_t f_tree_id;
            real_t f_tree_score;
            real_t f_tree_time;
            real_t f_tree_duration;
            real_t f_tree_frequency;
            real_t f_tree_slope;
            real_t f_tree_correlation;
            real_t f_tree_deviation;

            count_t f_size;
            real_t f_interval;
            const real_t* f_in;

            real_t* f_ratio;
            real_t* f_background;
            count_t f_frequency_minimum_index;
            count_t f_frequency_maximum_index;
            real_t f_minimum_time;
            real_t f_maximum_time;
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
                    static void set_ratio( real_t* p_ratio );
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
                    static real_t* s_ratio;
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
                    cluster( const real_t& p_time, const real_t& p_frequency );
                    ~cluster();

                public:
                    void update();

                    const count_t& id() const;
                    const real_t& count() const;
                    const real_t& score() const;

                    const real_t& time() const;
                    const real_t& duration() const;
                    const real_t& frequency() const;

                    const vector< real_t >& times() const;
                    const vector< real_t >& frequencies() const;
                    const vector< real_t >& ratios() const;
                    const vector< real_t >& gaps() const;

                private:
                    count_t f_id;
                    real_t f_count;
                    real_t f_score;

                    real_t f_time;
                    real_t f_duration;
                    real_t f_frequency;

                    vector< real_t > f_times;
                    vector< real_t > f_frequencies;
                    vector< real_t > f_ratios;
                    vector< real_t > f_gaps;

                    real_t f_w_sum;
                    real_t f_wt_sum;
                    real_t f_wf_sum;

                    real_t f_add_count_sum;
                    real_t f_add_score_sum;
                    real_t f_gap_count_sum;
                    real_t f_gap_score_sum;
                    real_t f_gap_count_current;
                    real_t f_gap_score_current;

                    typedef enum
                    {
                        e_in_add = 0,
                        e_to_add = 1,
                        e_in_gap = 2,
                        e_to_gap = 3
                    } state_t;
                    state_t f_state;
            };

            typedef list< cluster* > cluster_list;
            typedef cluster_list::iterator cluster_it;
            typedef cluster_list::const_iterator cluster_cit;

            cluster_list f_candidate_clusters;
            cluster_list f_complete_clusters;

        private:
            class line
            {
                public:
                    static void set_time( real_t* p_time );
                    static void set_ratio( real_t* p_ratio );
                    static void set_interval( const real_t& p_interval );
                    static void set_min_index( const count_t& p_size );
                    static void set_max_index( const count_t& p_size );
                    static void set_tolerance( const real_t& p_tolerance );
                    static void set_add_coefficient( const real_t& p_add_coefficient );
                    static void set_add_power( const real_t& p_add_power );
                    static void set_gap_coefficient( const real_t& p_gap_coefficient );
                    static void set_gap_power( const real_t& p_gap_power );
                    static void set_id( const count_t& p_id );

                private:
                    static real_t* s_time;
                    static real_t* s_ratio;
                    static count_t s_min_index;
                    static count_t s_max_index;
                    static real_t s_interval;
                    static real_t s_tolerance;
                    static real_t s_add_coefficient;
                    static real_t s_add_power;
                    static real_t s_gap_coefficient;
                    static real_t s_gap_power;
                    static count_t s_id;

                public:
                    line( const cluster& p_cluster );
                    ~line();

                public:
                    void update();

                    const count_t& id() const;
                    const real_t& count() const;
                    const real_t& score() const;


                    const real_t& time() const;
                    const real_t& duration() const;
                    const real_t& frequency() const;
                    const real_t& slope() const;
                    const real_t& correlation() const;
                    const real_t& deviation() const;

                    const vector< real_t >& times() const;
                    const vector< real_t >& frequencies() const;
                    const vector< real_t >& ratios() const;
                    const vector< real_t >& gaps() const;

                private:
                    count_t f_id;
                    real_t f_count;
                    real_t f_score;

                    real_t f_time;
                    real_t f_duration;
                    real_t f_frequency;
                    real_t f_slope;
                    real_t f_correlation;
                    real_t f_deviation;

                    vector< real_t > f_times;
                    vector< real_t > f_frequencies;
                    vector< real_t > f_ratios;
                    vector< real_t > f_gaps;

                    real_t f_w_sum;
                    real_t f_wt_sum;
                    real_t f_wf_sum;
                    real_t f_wtt_sum;
                    real_t f_wff_sum;
                    real_t f_wtf_sum;

                    real_t f_add_count_sum;
                    real_t f_add_score_sum;
                    real_t f_gap_count_sum;
                    real_t f_gap_score_sum;
                    real_t f_gap_count_current;
                    real_t f_gap_score_current;

                    typedef enum
                    {
                        e_in_add = 0,
                        e_to_add = 1,
                        e_in_gap = 2,
                        e_to_gap = 3
                    } state_t;
                    state_t f_state;
            };

            typedef list< line* > line_list;
            typedef line_list::iterator line_it;
            typedef line_list::const_iterator line_cit;

            line_list f_candidate_lines;
            line_list f_active_lines;
            line_list f_complete_lines;
    };

}

#endif
