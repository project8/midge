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

#include <stack>
using std::stack;

namespace midge
{

    class rf_event_consumer :
        public _consumer< rf_event_consumer, typelist_1( rf_data ) >
    {
        public:
            rf_event_consumer();
            ~rf_event_consumer();

        public:
            accessible( string, file_background )
            accessible( real_t, frequency_minimum )
            accessible( real_t, frequency_maximum )
            accessible( real_t, cluster_add_ratio )
            accessible( real_t, cluster_window )
            accessible( real_t, cluster_add_coefficient )
            accessible( real_t, cluster_add_power )
            accessible( real_t, cluster_gap_coefficient )
            accessible( real_t, cluster_gap_power )
            accessible( real_t, line_start_score )
            accessible( real_t, line_stop_score )
            accessible( real_t, line_window )
            accessible( real_t, line_width )
            accessible( real_t, line_add_coefficient )
            accessible( real_t, line_add_power )
            accessible( real_t, line_gap_coefficient )
            accessible( real_t, line_gap_power )
            accessible( real_t, event_time_tolerance )
            accessible( bool_t, plot_any )
            accessible( string, file_ratio )
            accessible( bool_t, plot_ratio )
            accessible( string, plot_ratio_key )
            accessible( string, plot_ratio_name )
            accessible( string, chart_ratio_title )
            accessible( string, axis_ratio_title )
            accessible( string, file_cluster )
            accessible( bool_t, plot_cluster )
            accessible( string, plot_cluster_key )
            accessible( string, plot_cluster_name )
            accessible( string, chart_cluster_title )
            accessible( string, axis_cluster_title )
            accessible( string, file_line )
            accessible( bool_t, plot_line )
            accessible( string, plot_line_key )
            accessible( string, plot_line_name )
            accessible( string, chart_line_title )
            accessible( string, axis_line_title )
            accessible( string, file_event )
            accessible( bool_t, plot_event )
            accessible( string, plot_event_key )
            accessible( string, plot_event_name )
            accessible( string, chart_event_title )
            accessible( string, axis_event_title )

        private:
            TFile* f_ratio_stream;
            TObjString* f_ratio_label;
            TTree* f_ratio_tree;
            real_t f_ratio_time;
            real_t f_ratio_frequency;
            real_t f_ratio_ratio;

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

            void write_clusters();
            void plot_cluster_data();
            void delete_clusters();

            void write_lines();
            void plot_line_data();
            void plot_line_lines();
            void delete_lines();

            void write_events();
            void plot_event_data();
            void plot_event_lines();
            void delete_events();

        private:
            class candidate
            {
                public:
                    candidate();
                    ~candidate();

                    referrable( real_t, time );
                    referrable( real_t, frequency );
                    referrable( real_t, weight );
            };

            typedef list< candidate* > candidate_list;
            typedef candidate_list::iterator candidate_it;
            typedef candidate_list::const_iterator candidate_cit;

            candidate_list f_active_candidates;
            candidate_list f_complete_candidates;

        private:
            class cluster
            {
                public:
                    static void set_time( real_t* p_time );
                    static void set_ratio( real_t* p_ratio );
                    static void set_interval( const real_t& p_interval );
                    static void set_min_index( const count_t& p_size );
                    static void set_max_index( const count_t& p_size );
                    static void set_window( const real_t& p_window );
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
                    static real_t s_window;
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
                    const real_t& time() const;
                    const real_t& duration() const;
                    const real_t& frequency() const;
                    const real_t& slope() const;
                    const real_t& correlation() const;
                    const real_t& deviation() const;
                    const real_t& occupation() const;
                    const real_t& score() const;

                    const vector< real_t >& times() const;
                    const vector< real_t >& frequencies() const;
                    const vector< real_t >& ratios() const;
                    const vector< real_t >& scores() const;
                    const vector< real_t >& gaps() const;

                private:
                    real_t weight( const real_t& p_frequency ) const;

                    count_t f_id;
                    real_t f_time;
                    real_t f_duration;
                    real_t f_frequency;
                    real_t f_slope;
                    real_t f_correlation;
                    real_t f_deviation;
                    real_t f_occupation;
                    real_t f_score;

                    vector< real_t > f_times;
                    vector< real_t > f_frequencies;
                    vector< real_t > f_ratios;
                    vector< real_t > f_scores;
                    vector< real_t > f_gaps;

                    real_t f_r_sum;
                    real_t f_rt_sum;
                    real_t f_rf_sum;
                    real_t f_rtt_sum;
                    real_t f_rff_sum;
                    real_t f_rtf_sum;

                    real_t f_add_occupation_sum;
                    real_t f_add_score_sum;
                    real_t f_gap_occupation_sum;
                    real_t f_gap_score_sum;
                    real_t f_gap_occupation_current;
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
            typedef stack< cluster_it > cluster_stack;

            cluster_list f_candidate_clusters;
            cluster_list f_complete_clusters;

            cluster_stack f_cluster_complete_stack;
            cluster_stack f_cluster_discard_stack;

        private:
            class line
            {
                public:
                    static void set_time( real_t* p_time );
                    static void set_ratio( real_t* p_ratio );
                    static void set_interval( const real_t& p_interval );
                    static void set_min_index( const count_t& p_size );
                    static void set_max_index( const count_t& p_size );
                    static void set_window( const real_t& p_window );
                    static void set_width( const real_t& p_width );
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
                    static real_t s_window;
                    static real_t s_width;
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
                    const real_t& time() const;
                    const real_t& duration() const;
                    const real_t& frequency() const;
                    const real_t& slope() const;
                    const real_t& correlation() const;
                    const real_t& deviation() const;
                    const real_t& occupation() const;
                    const real_t& score() const;

                    const vector< real_t >& times() const;
                    const vector< real_t >& frequencies() const;
                    const vector< real_t >& ratios() const;
                    const vector< real_t >& scores() const;
                    const vector< real_t >& gaps() const;

                private:
                    real_t weight( const real_t& p_frequency ) const;

                    count_t f_id;
                    real_t f_time;
                    real_t f_duration;
                    real_t f_frequency;
                    real_t f_slope;
                    real_t f_correlation;
                    real_t f_deviation;
                    real_t f_occupation;
                    real_t f_score;

                    vector< real_t > f_times;
                    vector< real_t > f_frequencies;
                    vector< real_t > f_ratios;
                    vector< real_t > f_scores;
                    vector< real_t > f_gaps;

                    real_t f_r_sum;
                    real_t f_rt_sum;
                    real_t f_rf_sum;
                    real_t f_rtt_sum;
                    real_t f_rff_sum;
                    real_t f_rtf_sum;

                    real_t f_add_occupation_sum;
                    real_t f_add_score_sum;
                    real_t f_gap_occupation_sum;
                    real_t f_gap_score_sum;
                    real_t f_gap_occupation_current;
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
            typedef stack< line_it > line_stack;

            line_list f_candidate_lines;
            line_list f_active_lines;
            line_list f_complete_lines;

            line_stack f_line_complete_stack;
            line_stack f_line_active_stack;
            line_stack f_line_discard_stack;

        private:
            class event
            {
                public:
                    static void set_id( const count_t& p_id );

                private:
                    static count_t s_id;

                public:
                    event( const line& p_line );
                    ~event();

                public:
                    void add( const line& p_line );

                    const count_t& id() const;
                    const real_t& time() const;
                    const real_t& duration() const;

                    const vector< real_t >& line_times() const;
                    const vector< real_t >& line_frequencies() const;

                    const vector< real_t >& data_times() const;
                    const vector< real_t >& data_frequencies() const;
                    const vector< real_t >& data_ratios() const;

                private:
                    count_t f_id;

                    real_t f_time;
                    real_t f_duration;

                    vector< real_t > f_line_times;
                    vector< real_t > f_line_frequencies;

                    vector< real_t > f_data_times;
                    vector< real_t > f_data_frequencies;
                    vector< real_t > f_data_ratios;
            };

            typedef list< event* > event_list;
            typedef event_list::iterator event_it;
            typedef event_list::const_iterator event_cit;
            typedef stack< event_it > event_stack;

            event_list f_active_events;
            event_list f_complete_events;

            event_stack f_event_complete_stack;
    };

}

#endif
