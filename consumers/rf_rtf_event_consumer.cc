#include "rf_rtf_event_consumer.hh"

#include "plot.hh"

#include <set>
using std::set;

#include <limits>
using std::numeric_limits;

#include <cmath>

namespace midge
{

    rf_rtf_event_consumer::rf_rtf_event_consumer() :
            f_file_background( "" ),
            f_frequency_minimum( 0. ),
            f_frequency_maximum( 0. ),
            f_candidate_ratio_threshold( -1. ),
            f_cluster_window( 1. ),
            f_cluster_add_coefficient( 1. ),
            f_cluster_add_power( 0. ),
            f_cluster_gap_coefficient( 2. ),
            f_cluster_gap_power( 0. ),
            f_cluster_score_threshold( 100. ),
            f_cluster_correlation_threshold( 1. ),
            f_line_stop_score( 100. ),
            f_line_tolerance( 1. ),
            f_line_add_coefficient( 1. ),
            f_line_add_power( 0. ),
            f_line_gap_coefficient( 2. ),
            f_line_gap_power( 0. ),
            f_event_time_tolerance( 0. ),
            f_plot_any( false ),
            f_file_ratio( "" ),
            f_plot_ratio( false ),
            f_plot_ratio_key( "" ),
            f_plot_ratio_name( "" ),
            f_chart_ratio_title( "" ),
            f_axis_ratio_title( "" ),
            f_file_cluster( "" ),
            f_plot_cluster( false ),
            f_plot_cluster_key( "" ),
            f_plot_cluster_name( "" ),
            f_chart_cluster_title( "" ),
            f_axis_cluster_title( "" ),
            f_file_line( "" ),
            f_plot_line( false ),
            f_plot_line_key( "" ),
            f_plot_line_name( "" ),
            f_chart_line_title( "" ),
            f_axis_line_title( "" ),
            f_file_event( "" ),
            f_plot_event( false ),
            f_plot_event_key( "" ),
            f_plot_event_name( "" ),
            f_chart_event_title( "" ),
            f_axis_event_title( "" ),
            f_ratio_stream( NULL ),
            f_ratio_label( NULL ),
            f_ratio_tree( NULL ),
            f_ratio_time( 0. ),
            f_ratio_frequency( 0. ),
            f_ratio_ratio( 0. ),
            f_size( 0 ),
            f_interval( 1. ),
            f_in( NULL ),
            f_ratio( NULL ),
            f_background( NULL ),
            f_frequency_minimum_index( 0 ),
            f_frequency_maximum_index( 0 ),
            f_minimum_time( 0. ),
            f_maximum_time( 0. ),
            f_count( 0 ),
            f_candidate_clusters(),
            f_complete_clusters(),
            f_candidate_lines(),
            f_active_lines(),
            f_complete_lines()
    {
    }
    rf_rtf_event_consumer::~rf_rtf_event_consumer()
    {
    }

    bool rf_rtf_event_consumer::start_consumer()
    {
        if( f_plot_any == true )
        {
            plot::get_instance()->initialize();
        }

        if( f_file_ratio.size() > 0 )
        {
            f_ratio_stream = new TFile( f_file_ratio.c_str(), "RECREATE" );
            f_ratio_label = new TObjString( "midge_ratios" );
            f_ratio_tree = new TTree( "ratios", "ratios" );
            f_ratio_tree->SetDirectory( f_ratio_stream );
            f_ratio_tree->Branch( "time", &f_ratio_time );
            f_ratio_tree->Branch( "frequency", &f_ratio_frequency );
            f_ratio_tree->Branch( "ratio", &f_ratio_ratio );
        }

        f_size = in< 0 >()->get_size();
        f_interval = in< 0 >()->get_interval();
        f_in = in< 0 >()->raw();

        f_ratio = new real_t[ f_size ];
        for( count_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_ratio[ t_index ] = -1.;
        }

        f_background = new real_t[ f_size ];
        for( count_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_background[ t_index ] = 0.;
        }

        if( f_file_background.size() == 0 )
        {
            throw error() << "cannot start event consumer <" << get_name() << "> with no background file set";
        }

        TFile* t_stream = new TFile( f_file_background.c_str(), "READ" );
        if( t_stream->IsZombie() == true )
        {
            throw error() << "cannot read background file";
        }

        TObjString* t_label = (TObjString*) (t_stream->Get( "midge_background" ));
        if( t_label == NULL )
        {
            throw error() << "background file has no label named <midge_background>";
        }

        TTree* t_tree = (TTree*) (t_stream->Get( "background" ));
        if( t_tree == NULL )
        {
            throw error() << "cannot read background tree";
        }

        real_t t_frequency;
        if( t_tree->SetBranchAddress( "frequency", &t_frequency ) == TTree::kMissingBranch )
        {
            throw error() << "background tree has no branch named <frequency>";
        }

        real_t t_value;
        if( t_tree->SetBranchAddress( "value", &t_value ) == TTree::kMissingBranch )
        {
            throw error() << "background tree has no branch named <value>";
        }

        Long64_t t_size = t_tree->GetEntries();
        if( t_size != f_size )
        {
            throw error() << "cannot analyze background tree with size <" << t_size << "> against input with size <" << f_size << ">";
        }

        for( Long64_t t_index = 0; t_index < t_size; t_index++ )
        {
            t_tree->GetEntry( t_index );
            f_background[ t_index ] = t_value;
        }

        t_stream->Close();
        delete t_stream;

        f_frequency_minimum_index = (count_t) (round( f_frequency_minimum / f_interval ));
        f_frequency_maximum_index = (count_t) (round( f_frequency_maximum / f_interval ));
        f_minimum_time = 0.;
        f_maximum_time = 0.;
        f_count = 0;

        cluster::set_time( &f_maximum_time );
        cluster::set_ratio( f_ratio );
        cluster::set_interval( f_interval );
        cluster::set_min_index( f_frequency_minimum_index );
        cluster::set_max_index( f_frequency_maximum_index );
        cluster::set_window( f_cluster_window );
        cluster::set_add_coefficient( f_cluster_add_coefficient );
        cluster::set_add_power( f_cluster_add_power );
        cluster::set_gap_coefficient( f_cluster_gap_coefficient );
        cluster::set_gap_power( f_cluster_gap_power );
        cluster::set_id( 0 );

        line::set_time( &f_maximum_time );
        line::set_ratio( f_ratio );
        line::set_interval( f_interval );
        line::set_min_index( f_frequency_minimum_index );
        line::set_max_index( f_frequency_maximum_index );
        line::set_tolerance( f_line_tolerance );
        line::set_add_coefficient( f_line_add_coefficient );
        line::set_add_power( f_line_add_power );
        line::set_gap_coefficient( f_line_gap_coefficient );
        line::set_gap_power( f_line_gap_power );
        line::set_id( 0 );

        return true;
    }

    bool rf_rtf_event_consumer::execute_consumer()
    {
        //msg_warning( coremsg, "** START **" << eom );

        if( f_count == 0 )
        {
            f_minimum_time = in< 0 >()->get_time();
        }
        f_maximum_time = in< 0 >()->get_time();
        f_count++;

        //msg_warning( coremsg, "** updating ratios **" << eom );
        register double t_in;
        register double t_background;
        register double t_ratio;
        for( count_t t_index = f_frequency_minimum_index; t_index <= f_frequency_maximum_index; t_index++ )
        {
            t_in = f_in[ t_index ];
            t_background = f_background[ t_index ];
            t_ratio = (t_in - t_background) / t_background;
            if( t_ratio < f_candidate_ratio_threshold )
            {
                f_ratio[ t_index ] = -1.;
            }
            else
            {
                f_ratio[ t_index ] = t_ratio - f_candidate_ratio_threshold;

                f_ratio_time = f_maximum_time;
                f_ratio_frequency = f_interval * t_index;
                f_ratio_ratio = t_ratio;
                f_ratio_tree->Fill();
            }
        }

        //msg_warning( coremsg, "** updating lines **" << eom );
        line_it t_line_it;
        line* t_line;
        for( t_line_it = f_active_lines.begin(); t_line_it != f_active_lines.end(); t_line_it++ )
        {
            t_line = *t_line_it;

            //msg_warning( coremsg, "**   updating active line <" << t_line->id() << "> **" << eom );
            t_line->update();

            if( t_line->score() < f_line_stop_score )
            {
                //msg_warning( coremsg, "**   will complete active line <" << t_line->id() << "> **" << eom );
                f_line_complete_stack.push( t_line_it );
            }
        }
        for( t_line_it = f_candidate_lines.begin(); t_line_it != f_candidate_lines.end(); t_line_it++ )
        {
            t_line = *t_line_it;

            //msg_warning( coremsg, "**   updating candidate line <" << t_line->id() << "> **" << eom );
            t_line->update();

            if( t_line->score() > f_line_stop_score )
            {
                //msg_warning( coremsg, "**   will activate candidate line <" << t_line->id() << "> **" << eom );
                f_line_active_stack.push( t_line_it );
            }
            if( t_line->score() < 0. )
            {
                //msg_warning( coremsg, "**   will discard candidate line <" << t_line->id() << "> **" << eom );
                f_line_discard_stack.push( t_line_it );
            }
        }
        //msg_warning( coremsg, "** lines updated **" << eom );

        //msg_warning( coremsg, "** updating clusters **" << eom );
        cluster_it t_cluster_it;
        cluster* t_cluster;
        for( t_cluster_it = f_candidate_clusters.begin(); t_cluster_it != f_candidate_clusters.end(); t_cluster_it++ )
        {
            t_cluster = *t_cluster_it;

            //msg_warning( coremsg, "**   updating candidate cluster <" << t_cluster->id() << "> **" << eom );
            t_cluster->update();

            if( (t_cluster->score() > f_cluster_score_threshold) && (t_cluster->correlation() > f_cluster_correlation_threshold) )
            {
                //msg_warning( coremsg, "**   will complete candidate cluster <" << t_cluster->id() << "> **" << eom );
                f_cluster_complete_stack.push( t_cluster_it );
            }

            if( t_cluster->score() < 0. )
            {
                //msg_warning( coremsg, "**   will discard candidate cluster <" << t_cluster->id() << "> **" << eom );
                f_cluster_discard_stack.push( t_cluster_it );
            }
        }
        //msg_warning( coremsg, "** clusters updated **" << eom );

        //msg_warning( coremsg, "** completing lines **" << eom );
        event_it t_event_it;
        event* t_event;
        while( f_line_complete_stack.empty() == false )
        {
            t_line = *(f_line_complete_stack.top());
            f_complete_lines.push_back( t_line );
            //msg_warning( coremsg, "**   completing line <" << t_line->id() << "> **" << eom );

            t_event_it = f_active_events.begin();
            while( true )
            {
                if( t_event_it == f_active_events.end() )
                {
                    f_active_events.push_back( new event( *t_line ) );
                    break;
                }

                t_event = *t_event_it;

                if( t_line->time() - f_event_time_tolerance > t_event->time() + t_event->duration() )
                {
                    //f_event_complete_stack.push( t_event_it );
                    t_event_it++;
                    continue;
                }

                if( t_line->time() + f_event_time_tolerance < t_event->time() + t_event->duration() )
                {
                    t_event_it++;
                    continue;
                }

                t_event->add( *t_line );
                break;
            }
            while( f_event_complete_stack.empty() == false )
            {
                t_event = *(f_event_complete_stack.top());
                f_complete_events.push_back( t_event );

                f_active_events.erase( f_event_complete_stack.top() );
                f_event_complete_stack.pop();
            }

            f_active_lines.erase( f_line_complete_stack.top() );
            f_line_complete_stack.pop();
        }

        //msg_warning( coremsg, "** activating lines **" << eom );
        while( f_line_active_stack.empty() == false )
        {
            t_line = *(f_line_active_stack.top());
            f_active_lines.push_back( t_line );
            //msg_warning( coremsg, "**   activating line <" << t_line->id() << "> **" << eom );

            f_candidate_lines.erase( f_line_active_stack.top() );
            f_line_active_stack.pop();
        }

        //msg_warning( coremsg, "** discarding lines **" << eom );
        while( f_line_discard_stack.empty() == false )
        {
            t_line = *(f_line_discard_stack.top());
            //msg_warning( coremsg, "**   discarding line <" << t_line->id() << "> **" << eom );

            f_candidate_lines.erase( f_line_discard_stack.top() );
            f_line_discard_stack.pop();

            delete t_line;
        }

        //msg_warning( coremsg, "** completing clusters **" << eom );
        while( f_cluster_complete_stack.empty() == false )
        {
            t_cluster = *(f_cluster_complete_stack.top());
            f_complete_clusters.push_back( t_cluster );
            //msg_warning( coremsg, "**   completing cluster <" << t_cluster->id() << "> **" << eom );

            f_candidate_clusters.erase( f_cluster_complete_stack.top() );
            f_cluster_complete_stack.pop();

            t_line = new line( *t_cluster );
            if( t_line->score() < 0. )
            {
                delete t_line;
            }
            else
            {
                f_candidate_lines.push_back( t_line );
            }
        }

        //msg_warning( coremsg, "** discarding clusters **" << eom );
        while( f_cluster_discard_stack.empty() == false )
        {
            t_cluster = *(f_cluster_discard_stack.top());
            //msg_warning( coremsg, "**   discarding cluster <" << t_cluster->id() << "> **" << eom );

            f_candidate_clusters.erase( f_cluster_discard_stack.top() );
            f_cluster_discard_stack.pop();

            delete t_cluster;
        }

        //msg_warning( coremsg, "** adding clusters **" << eom );
        for( count_t t_index = f_frequency_minimum_index; t_index <= f_frequency_maximum_index; t_index++ )
        {
            if( f_ratio[ t_index ] > 0. )
            {
                t_cluster = new cluster( f_maximum_time, t_index * f_interval );
                f_candidate_clusters.push_back( t_cluster );
            }
        }

        //msg_warning( coremsg, "** FINISH **" << eom );
        return true;
    }

    bool rf_rtf_event_consumer::stop_consumer()
    {
        if( f_ratio_stream != NULL )
        {
            if( (f_plot_any == true) && (f_plot_ratio == true) )
            {
                Long64_t t_ratio_count = f_ratio_tree->GetEntries();

                plot::abscissa t_ratio_times( t_ratio_count );
                t_ratio_times.title() = string( "Time [sec]" );
                t_ratio_times.count() = f_count;
                t_ratio_times.low() = f_minimum_time;
                t_ratio_times.high() = f_maximum_time;

                plot::abscissa t_ratio_frequencies( t_ratio_count );
                t_ratio_frequencies.title() = string( "Frequency [Hz]" );
                t_ratio_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
                t_ratio_frequencies.low() = f_frequency_minimum_index * f_interval;
                t_ratio_frequencies.high() = f_frequency_maximum_index * f_interval;

                plot::ordinate t_ratio_values( t_ratio_count );
                t_ratio_values.title() = f_axis_ratio_title;

                for( Long64_t t_index = 0; t_index < t_ratio_count; t_index++ )
                {
                    f_ratio_tree->GetEntry( t_index );

                    t_ratio_times.values().at( t_index ) = f_ratio_time;
                    t_ratio_frequencies.values().at( t_index ) = f_ratio_frequency;
                    t_ratio_values.values().at( t_index ) = f_ratio_ratio;
                }

                plot::get_instance()->plot_two_dimensional( f_plot_ratio_key, f_plot_ratio_name, f_chart_ratio_title, t_ratio_times, t_ratio_frequencies, t_ratio_values );
            }

            f_ratio_stream->cd();
            f_ratio_label->Write();
            f_ratio_tree->Write();
            f_ratio_stream->Close();
            delete f_ratio_stream;
            f_ratio_label = NULL;
            f_ratio_tree = NULL;
            f_ratio_stream = NULL;
        }

        write_clusters();
        plot_cluster_data();
        delete_clusters();

        write_lines();
        plot_line_data();
        plot_line_lines();
        delete_lines();

        write_events();
        plot_event_data();
        plot_event_lines();
        delete_events();

        if( f_plot_any == true )
        {
            plot::get_instance()->finalize();
        }

        f_size = 0;
        f_interval = 1.;
        f_in = NULL;

        delete[] f_ratio;
        f_ratio = NULL;

        delete[] f_background;
        f_background = NULL;

        f_frequency_minimum_index = 0;
        f_frequency_maximum_index = 0;
        f_minimum_time = 0.;
        f_maximum_time = 0.;
        f_count = 0;

        cluster::set_time( NULL );
        cluster::set_ratio( NULL );
        cluster::set_interval( 1. );
        cluster::set_min_index( 0 );
        cluster::set_max_index( 0 );
        cluster::set_window( 1. );
        cluster::set_add_coefficient( 1. );
        cluster::set_add_power( 0. );
        cluster::set_gap_coefficient( 2. );
        cluster::set_gap_power( 0. );
        cluster::set_id( 0 );

        line::set_time( NULL );
        line::set_ratio( NULL );
        line::set_interval( 1. );
        line::set_min_index( 0 );
        line::set_max_index( 0 );
        line::set_tolerance( 1. );
        line::set_add_coefficient( 1. );
        line::set_add_power( 0. );
        line::set_gap_coefficient( 2. );
        line::set_gap_power( 0. );
        line::set_id( 0 );

        return true;
    }

    void rf_rtf_event_consumer::write_clusters()
    {
        if( f_file_cluster.size() > 0 )
        {
            TFile* t_cluster_stream = new TFile( f_file_cluster.c_str(), "RECREATE" );
            TObjString* t_cluster_label = new TObjString( "midge_clusters" );
            TTree* t_cluster_tree = new TTree( "clusters", "clusters" );
            count_t t_tree_id;
            real_t t_tree_score;
            real_t t_tree_time;
            real_t t_tree_duration;
            real_t t_tree_frequency;
            real_t t_tree_slope;
            real_t t_tree_correlation;
            real_t t_tree_deviation;
            t_cluster_tree->SetDirectory( t_cluster_stream );
            t_cluster_tree->Branch( "id", &t_tree_id );
            t_cluster_tree->Branch( "score", &t_tree_score );
            t_cluster_tree->Branch( "time", &t_tree_time );
            t_cluster_tree->Branch( "duration", &t_tree_duration );
            t_cluster_tree->Branch( "frequency", &t_tree_frequency );
            t_cluster_tree->Branch( "slope", &t_tree_slope );
            t_cluster_tree->Branch( "correlation", &t_tree_correlation );
            t_cluster_tree->Branch( "deviation", &t_tree_deviation );

            TTree* t_cluster_data_tree;
            stringstream t_cluster_tree_name;
            real_t t_tree_data_time;
            real_t t_tree_data_frequency;
            real_t t_tree_data_ratio;
            real_t t_tree_data_score;

            cluster* t_cluster;
            for( cluster_it t_it = f_complete_clusters.begin(); t_it != f_complete_clusters.end(); t_it++ )
            {
                t_cluster = *t_it;

                t_tree_id = t_cluster->id();
                t_tree_score = t_cluster->score();
                t_tree_time = t_cluster->time();
                t_tree_duration = t_cluster->duration();
                t_tree_frequency = t_cluster->frequency();
                t_tree_slope = t_cluster->slope();
                t_tree_correlation = t_cluster->correlation();
                t_tree_deviation = t_cluster->deviation();
                t_cluster_tree->Fill();

                t_cluster_tree_name << "cluster_" << t_tree_id << "_data";
                t_cluster_data_tree = new TTree( t_cluster_tree_name.str().c_str(), t_cluster_tree_name.str().c_str() );
                t_cluster_data_tree->SetDirectory( t_cluster_stream );
                t_cluster_data_tree->Branch( "time", &t_tree_data_time );
                t_cluster_data_tree->Branch( "frequency", &t_tree_data_frequency );
                t_cluster_data_tree->Branch( "ratio", &t_tree_data_ratio );
                t_cluster_data_tree->Branch( "score", &t_tree_data_score );
                for( count_t t_index = 0; t_index < t_cluster->ratios().size(); t_index++ )
                {
                    t_tree_data_time = t_cluster->times().at( t_index );
                    t_tree_data_frequency = t_cluster->frequencies().at( t_index );
                    t_tree_data_ratio = t_cluster->ratios().at( t_index );
                    t_cluster_data_tree->Fill();
                }
                t_cluster_stream->cd();
                t_cluster_data_tree->Write();

                t_cluster_tree_name.str( "" );
                t_cluster_tree_name.clear();
            }

            t_cluster_stream->cd();
            t_cluster_label->Write();
            t_cluster_tree->Write();
            t_cluster_stream->Close();
            delete t_cluster_stream;
            t_cluster_label = NULL;
            t_cluster_tree = NULL;
            t_cluster_stream = NULL;
        }
        return;
    }
    void rf_rtf_event_consumer::plot_cluster_data()
    {
        if( (f_plot_any == true) && (f_plot_cluster == true) )
        {
            cluster* t_cluster;

            count_t t_count = 0;
            vector< real_t > t_times;
            vector< real_t > t_frequencies;
            vector< real_t > t_scores;
            for( cluster_it t_it = f_complete_clusters.begin(); t_it != f_complete_clusters.end(); t_it++ )
            {
                t_cluster = *t_it;

                t_count += t_cluster->times().size();
                t_times.insert( t_times.end(), t_cluster->times().begin(), t_cluster->times().end() );
                t_frequencies.insert( t_frequencies.end(), t_cluster->frequencies().begin(), t_cluster->frequencies().end() );
                t_scores.insert( t_scores.end(), t_cluster->scores().begin(), t_cluster->scores().end() );
            }

            plot::abscissa t_cluster_data_times( t_count );
            t_cluster_data_times.title() = string( "Time [sec]" );
            t_cluster_data_times.count() = f_count;
            t_cluster_data_times.low() = f_minimum_time;
            t_cluster_data_times.high() = f_maximum_time;
            t_cluster_data_times.values() = t_times;

            plot::abscissa t_cluster_data_frequencies( t_count );
            t_cluster_data_frequencies.title() = string( "Frequency [Hz]" );
            t_cluster_data_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
            t_cluster_data_frequencies.low() = f_frequency_minimum_index * f_interval;
            t_cluster_data_frequencies.high() = f_frequency_maximum_index * f_interval;
            t_cluster_data_frequencies.values() = t_frequencies;

            plot::ordinate t_cluster_data_scores( t_count );
            t_cluster_data_scores.title() = f_axis_cluster_title;
            t_cluster_data_scores.values() = t_scores;

            plot::get_instance()->plot_two_dimensional( f_plot_cluster_key, f_plot_cluster_name, f_chart_cluster_title, t_cluster_data_times, t_cluster_data_frequencies, t_cluster_data_scores );
        }
        return;
    }
    void rf_rtf_event_consumer::delete_clusters()
    {
        cluster_it t_cluster_it;
        cluster* t_cluster;
        while( f_candidate_clusters.empty() == false )
        {
            t_cluster_it = f_candidate_clusters.begin();
            t_cluster = (*t_cluster_it);
            f_candidate_clusters.pop_front();
            delete t_cluster;
        }
        while( f_complete_clusters.empty() == false )
        {
            t_cluster_it = f_complete_clusters.begin();
            t_cluster = (*t_cluster_it);
            f_complete_clusters.pop_front();
            delete t_cluster;
        }
    }

    void rf_rtf_event_consumer::write_lines()
    {
        if( f_file_line.size() > 0 )
        {
            TFile* t_line_stream = new TFile( f_file_line.c_str(), "RECREATE" );
            TObjString* t_line_label = new TObjString( "midge_lines" );
            TTree* t_line_tree = new TTree( "lines", "lines" );
            count_t t_tree_id;
            real_t t_tree_score;
            real_t t_tree_time;
            real_t t_tree_duration;
            real_t t_tree_frequency;
            real_t t_tree_slope;
            real_t t_tree_correlation;
            real_t t_tree_deviation;
            t_line_tree->SetDirectory( t_line_stream );
            t_line_tree->Branch( "id", &t_tree_id );
            t_line_tree->Branch( "score", &t_tree_score );
            t_line_tree->Branch( "time", &t_tree_time );
            t_line_tree->Branch( "duration", &t_tree_duration );
            t_line_tree->Branch( "frequency", &t_tree_frequency );
            t_line_tree->Branch( "slope", &t_tree_slope );
            t_line_tree->Branch( "correlation", &t_tree_correlation );
            t_line_tree->Branch( "deviation", &t_tree_deviation );

            stringstream t_line_data_name;
            TTree* t_line_data_tree;
            real_t t_tree_data_time;
            real_t t_tree_data_frequency;
            real_t t_tree_data_ratio;

            line* t_line;
            for( line_it t_it = f_complete_lines.begin(); t_it != f_complete_lines.end(); t_it++ )
            {
                t_line = *t_it;

                t_tree_id = t_line->id();
                t_tree_score = t_line->score();
                t_tree_time = t_line->time();
                t_tree_duration = t_line->duration();
                t_tree_frequency = t_line->frequency();
                t_tree_slope = t_line->slope();
                t_tree_correlation = t_line->correlation();
                t_tree_deviation = t_line->deviation();
                t_line_tree->Fill();

                t_line_data_name << "line_" << t_tree_id << "_data";
                t_line_data_tree = new TTree( t_line_data_name.str().c_str(), t_line_data_name.str().c_str() );
                t_line_data_tree->SetDirectory( t_line_stream );
                t_line_data_tree->Branch( "time", &t_tree_data_time );
                t_line_data_tree->Branch( "frequency", &t_tree_data_frequency );
                t_line_data_tree->Branch( "ratio", &t_tree_data_ratio );
                for( count_t t_index = 0; t_index < t_line->ratios().size(); t_index++ )
                {
                    t_tree_data_time = t_line->times().at( t_index );
                    t_tree_data_frequency = t_line->frequencies().at( t_index );
                    t_tree_data_ratio = t_line->ratios().at( t_index );
                    t_line_data_tree->Fill();
                }
                t_line_stream->cd();
                t_line_data_tree->Write();

                t_line_data_name.str( "" );
                t_line_data_name.clear();
            }

            t_line_stream->cd();
            t_line_label->Write();
            t_line_tree->Write();
            t_line_stream->Close();
            delete t_line_stream;
            t_line_label = NULL;
            t_line_tree = NULL;
            t_line_stream = NULL;
        }
        return;
    }
    void rf_rtf_event_consumer::plot_line_data()
    {
        if( (f_plot_any == true) && (f_plot_line == true) )
        {
            line* t_line;

            count_t t_count = 0;
            vector< real_t > t_times;
            vector< real_t > t_frequencies;
            vector< real_t > t_scores;
            for( line_it t_it = f_complete_lines.begin(); t_it != f_complete_lines.end(); t_it++ )
            {
                t_line = *t_it;

                t_count += t_line->times().size();
                t_times.insert( t_times.end(), t_line->times().begin(), t_line->times().end() );
                t_frequencies.insert( t_frequencies.end(), t_line->frequencies().begin(), t_line->frequencies().end() );
                t_scores.insert( t_scores.end(), t_line->scores().begin(), t_line->scores().end() );
            }

            plot::abscissa t_line_data_times( t_count );
            t_line_data_times.title() = string( "Time [sec]" );
            t_line_data_times.count() = f_count;
            t_line_data_times.low() = f_minimum_time;
            t_line_data_times.high() = f_maximum_time;
            t_line_data_times.values() = t_times;

            plot::abscissa t_line_data_frequencies( t_count );
            t_line_data_frequencies.title() = string( "Frequency [Hz]" );
            t_line_data_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
            t_line_data_frequencies.low() = f_frequency_minimum_index * f_interval;
            t_line_data_frequencies.high() = f_frequency_maximum_index * f_interval;
            t_line_data_frequencies.values() = t_frequencies;

            plot::ordinate t_line_data_values( t_count );
            t_line_data_values.title() = f_axis_line_title;
            t_line_data_values.values() = t_scores;

            plot::get_instance()->plot_two_dimensional( f_plot_line_key, f_plot_line_name, f_chart_line_title, t_line_data_times, t_line_data_frequencies, t_line_data_values );
        }
        return;
    }
    void rf_rtf_event_consumer::plot_line_lines()
    {
        if( (f_plot_any == true) && (f_plot_line == true) )
        {
            plot::abscissa t_line_line_times;
            plot::abscissa t_line_line_frequencies;

            line* t_line;
            for( line_it t_it = f_complete_lines.begin(); t_it != f_complete_lines.end(); t_it++ )
            {
                t_line = *t_it;

                t_line_line_times( 2 );
                t_line_line_times.title() = string( "Time [sec]" );
                t_line_line_times.count() = f_count;
                t_line_line_times.low() = f_minimum_time;
                t_line_line_times.high() = f_maximum_time;
                t_line_line_times.values().at( 0 ) = t_line->time();
                t_line_line_times.values().at( 1 ) = t_line->time() + t_line->duration();

                t_line_line_frequencies( 2 );
                t_line_line_frequencies.title() = string( "Frequency [Hz]" );
                t_line_line_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
                t_line_line_frequencies.low() = f_frequency_minimum_index * f_interval;
                t_line_line_frequencies.high() = f_frequency_maximum_index * f_interval;
                t_line_line_frequencies.values().at( 0 ) = t_line->frequency();
                t_line_line_frequencies.values().at( 1 ) = t_line->frequency() + t_line->slope() * t_line->duration();

                plot::get_instance()->graph_two_dimensional( f_plot_line_key, f_plot_line_name, f_chart_line_title, t_line_line_times, t_line_line_frequencies );
            }
        }
        return;
    }
    void rf_rtf_event_consumer::delete_lines()
    {
        line_it t_line_it;
        line* t_line;
        while( f_candidate_lines.empty() == false )
        {
            t_line_it = f_candidate_lines.begin();
            t_line = (*t_line_it);
            f_candidate_lines.pop_front();
            delete t_line;
        }
        while( f_active_lines.empty() == false )
        {
            t_line_it = f_active_lines.begin();
            t_line = (*t_line_it);
            f_active_lines.pop_front();
            delete t_line;
        }
        while( f_complete_lines.empty() == false )
        {
            t_line_it = f_complete_lines.begin();
            t_line = (*t_line_it);
            f_complete_lines.pop_front();
            delete t_line;
        }
    }

    void rf_rtf_event_consumer::write_events()
    {
        if( f_file_event.size() > 0 )
        {
            TFile* t_event_stream = new TFile( f_file_event.c_str(), "RECREATE" );
            TObjString* t_event_label = new TObjString( "midge_events" );
            TTree* t_event_tree = new TTree( "events", "events" );
            t_event_tree->SetDirectory( t_event_stream );
            count_t t_tree_id;
            real_t t_tree_time;
            real_t t_tree_duration;
            t_event_tree->Branch( "id", &t_tree_id );
            t_event_tree->Branch( "time", &t_tree_time );
            t_event_tree->Branch( "duration", &t_tree_duration );

            stringstream t_event_data_name;
            TTree* t_event_data_tree;
            real_t t_tree_data_time;
            real_t t_tree_data_frequency;
            real_t t_tree_data_ratio;

            stringstream t_event_line_name;
            TTree* t_event_line_tree;
            real_t t_tree_line_time;
            real_t t_tree_line_frequency;

            event* t_event;
            for( event_it t_it = f_active_events.begin(); t_it != f_active_events.end(); t_it++ )
            {
                t_event = *t_it;

                t_tree_id = t_event->id();
                t_tree_time = t_event->time();
                t_tree_duration = t_event->duration();
                t_event_tree->Fill();

                t_event_data_name << "event_" << t_tree_id << "_data";
                t_event_data_tree = new TTree( t_event_data_name.str().c_str(), t_event_data_name.str().c_str() );
                t_event_data_tree->SetDirectory( t_event_stream );
                t_event_data_tree->Branch( "time", &t_tree_data_time );
                t_event_data_tree->Branch( "frequency", &t_tree_data_frequency );
                t_event_data_tree->Branch( "ratio", &t_tree_data_ratio );
                for( count_t t_index = 0; t_index < t_event->data_ratios().size(); t_index++ )
                {
                    t_tree_data_time = t_event->data_times().at( t_index );
                    t_tree_data_frequency = t_event->data_frequencies().at( t_index );
                    t_tree_data_ratio = t_event->data_ratios().at( t_index );
                    t_event_data_tree->Fill();
                }

                t_event_stream->cd();
                t_event_data_tree->Write();

                t_event_data_name.str( "" );
                t_event_data_name.clear();

                t_event_line_name << "event_" << t_tree_id << "_lines";
                t_event_line_tree = new TTree( t_event_line_name.str().c_str(), t_event_line_name.str().c_str() );
                t_event_line_tree->SetDirectory( t_event_stream );
                t_event_line_tree->Branch( "time", &t_tree_line_time );
                t_event_line_tree->Branch( "frequency", &t_tree_line_frequency );
                for( count_t t_index = 0; t_index < t_event->line_times().size(); t_index++ )
                {
                    t_tree_line_time = t_event->line_times().at( t_index );
                    t_tree_line_frequency = t_event->line_frequencies().at( t_index );
                    t_event_line_tree->Fill();
                }
                t_event_stream->cd();
                t_event_line_tree->Write();

                t_event_line_name.str( "" );
                t_event_line_name.clear();
            }
            for( event_it t_it = f_complete_events.begin(); t_it != f_complete_events.end(); t_it++ )
            {
                t_event = *t_it;

                t_tree_id = t_event->id();
                t_tree_time = t_event->time();
                t_tree_duration = t_event->duration();
                t_event_tree->Fill();

                t_event_data_name << "event_" << t_tree_id << "_data";
                t_event_data_tree = new TTree( t_event_data_name.str().c_str(), t_event_data_name.str().c_str() );
                t_event_data_tree->SetDirectory( t_event_stream );
                t_event_data_tree->Branch( "time", &t_tree_data_time );
                t_event_data_tree->Branch( "frequency", &t_tree_data_frequency );
                t_event_data_tree->Branch( "ratio", &t_tree_data_ratio );
                for( count_t t_index = 0; t_index < t_event->data_ratios().size(); t_index++ )
                {
                    t_tree_data_time = t_event->data_times().at( t_index );
                    t_tree_data_frequency = t_event->data_frequencies().at( t_index );
                    t_tree_data_ratio = t_event->data_ratios().at( t_index );
                    t_event_data_tree->Fill();
                }

                t_event_stream->cd();
                t_event_data_tree->Write();

                t_event_data_name.str( "" );
                t_event_data_name.clear();

                t_event_line_name << "event_" << t_tree_id << "_lines";
                t_event_line_tree = new TTree( t_event_line_name.str().c_str(), t_event_line_name.str().c_str() );
                t_event_line_tree->SetDirectory( t_event_stream );
                t_event_line_tree->Branch( "time", &t_tree_line_time );
                t_event_line_tree->Branch( "frequency", &t_tree_line_frequency );
                for( count_t t_index = 0; t_index < t_event->line_times().size(); t_index++ )
                {
                    t_tree_line_time = t_event->line_times().at( t_index );
                    t_tree_line_frequency = t_event->line_frequencies().at( t_index );
                    t_event_line_tree->Fill();
                }
                t_event_stream->cd();
                t_event_line_tree->Write();

                t_event_line_name.str( "" );
                t_event_line_name.clear();
            }

            t_event_stream->cd();
            t_event_label->Write();
            t_event_tree->Write();
            t_event_stream->Close();
            delete t_event_stream;
            t_event_label = NULL;
            t_event_tree = NULL;
            t_event_stream = NULL;
        }
        return;
    }
    void rf_rtf_event_consumer::plot_event_data()
    {
        if( (f_plot_any == true) && (f_plot_event == true) )
        {
            event* t_event;

            count_t t_count = 0;
            vector< real_t > t_times;
            vector< real_t > t_frequencies;
            vector< real_t > t_ratios;
            for( event_it t_it = f_complete_events.begin(); t_it != f_complete_events.end(); t_it++ )
            {
                t_event = *t_it;
                t_count += t_event->data_times().size();
                t_times.insert( t_times.end(), t_event->data_times().begin(), t_event->data_times().end() );
                t_frequencies.insert( t_frequencies.end(), t_event->data_frequencies().begin(), t_event->data_frequencies().end() );
                t_ratios.insert( t_ratios.end(), t_event->data_ratios().begin(), t_event->data_ratios().end() );
            }
            for( event_it t_it = f_active_events.begin(); t_it != f_active_events.end(); t_it++ )
            {
                t_event = *t_it;
                t_count += t_event->data_times().size();
                t_times.insert( t_times.end(), t_event->data_times().begin(), t_event->data_times().end() );
                t_frequencies.insert( t_frequencies.end(), t_event->data_frequencies().begin(), t_event->data_frequencies().end() );
                t_ratios.insert( t_ratios.end(), t_event->data_ratios().begin(), t_event->data_ratios().end() );
            }

            plot::abscissa t_event_data_times( t_count );
            t_event_data_times.title() = string( "Time [sec]" );
            t_event_data_times.count() = f_count;
            t_event_data_times.low() = f_minimum_time;
            t_event_data_times.high() = f_maximum_time;
            t_event_data_times.values() = t_times;

            plot::abscissa t_event_data_frequencies( t_count );
            t_event_data_frequencies.title() = string( "Frequency [Hz]" );
            t_event_data_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
            t_event_data_frequencies.low() = f_frequency_minimum_index * f_interval;
            t_event_data_frequencies.high() = f_frequency_maximum_index * f_interval;
            t_event_data_frequencies.values() = t_frequencies;

            plot::ordinate t_event_data_values( t_count );
            t_event_data_values.title() = f_axis_event_title;
            t_event_data_values.values() = t_ratios;

            plot::get_instance()->plot_two_dimensional( f_plot_event_key, f_plot_event_name, f_chart_event_title, t_event_data_times, t_event_data_frequencies, t_event_data_values );
        }
        return;
    }
    void rf_rtf_event_consumer::plot_event_lines()
    {
        if( (f_plot_any == true) && (f_plot_event == true) )
        {
            plot::abscissa t_event_line_times;
            plot::abscissa t_event_line_frequencies;

            event* t_event;
            for( event_it t_it = f_complete_events.begin(); t_it != f_complete_events.end(); t_it++ )
            {
                t_event = *t_it;

                t_event_line_times( t_event->line_times().size() );
                t_event_line_times.title() = string( "Time [sec]" );
                t_event_line_times.count() = f_count;
                t_event_line_times.low() = f_minimum_time;
                t_event_line_times.high() = f_maximum_time;
                t_event_line_times.values() = t_event->line_times();

                t_event_line_frequencies( t_event->line_frequencies().size() );
                t_event_line_frequencies.title() = string( "Frequency [Hz]" );
                t_event_line_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
                t_event_line_frequencies.low() = f_frequency_minimum_index * f_interval;
                t_event_line_frequencies.high() = f_frequency_maximum_index * f_interval;
                t_event_line_frequencies.values() = t_event->line_frequencies();

                plot::get_instance()->graph_two_dimensional( f_plot_event_key, f_plot_event_name, f_chart_event_title, t_event_line_times, t_event_line_frequencies );
            }
            for( event_it t_it = f_active_events.begin(); t_it != f_active_events.end(); t_it++ )
            {
                t_event = *t_it;

                t_event_line_times( t_event->line_times().size() );
                t_event_line_times.title() = string( "Time [sec]" );
                t_event_line_times.count() = f_count;
                t_event_line_times.low() = f_minimum_time;
                t_event_line_times.high() = f_maximum_time;
                t_event_line_times.values() = t_event->line_times();

                t_event_line_frequencies( t_event->line_frequencies().size() );
                t_event_line_frequencies.title() = string( "Frequency [Hz]" );
                t_event_line_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
                t_event_line_frequencies.low() = f_frequency_minimum_index * f_interval;
                t_event_line_frequencies.high() = f_frequency_maximum_index * f_interval;
                t_event_line_frequencies.values() = t_event->line_frequencies();

                plot::get_instance()->graph_two_dimensional( f_plot_event_key, f_plot_event_name, f_chart_event_title, t_event_line_times, t_event_line_frequencies );
            }
        }
        return;
    }
    void rf_rtf_event_consumer::delete_events()
    {
        event_it t_event_it;
        event* t_event;
        while( f_active_events.empty() == false )
        {
            t_event_it = f_active_events.begin();
            t_event = (*t_event_it);
            f_active_events.pop_front();
            delete t_event;
        }
        while( f_complete_events.empty() == false )
        {
            t_event_it = f_complete_events.begin();
            t_event = (*t_event_it);
            f_complete_events.pop_front();
            delete t_event;
        }
    }

    //*******
    //cluster
    //*******

    void rf_rtf_event_consumer::cluster::set_time( real_t* p_time )
    {
        s_time = p_time;
        return;
    }
    void rf_rtf_event_consumer::cluster::set_ratio( real_t* p_ratio )
    {
        s_ratio = p_ratio;
        return;
    }
    void rf_rtf_event_consumer::cluster::set_interval( const real_t& p_interval )
    {
        s_interval = p_interval;
        return;
    }
    void rf_rtf_event_consumer::cluster::set_min_index( const count_t& p_min_index )
    {
        s_min_index = p_min_index;
        return;
    }
    void rf_rtf_event_consumer::cluster::set_max_index( const count_t& p_max_index )
    {
        s_max_index = p_max_index;
        return;
    }
    void rf_rtf_event_consumer::cluster::set_window( const real_t& p_window )
    {
        s_window = p_window;
        return;
    }
    void rf_rtf_event_consumer::cluster::set_add_coefficient( const real_t& p_add_coefficient )
    {
        s_add_coefficient = p_add_coefficient;
        return;
    }
    void rf_rtf_event_consumer::cluster::set_add_power( const real_t& p_add_power )
    {
        s_add_power = p_add_power;
        return;
    }
    void rf_rtf_event_consumer::cluster::set_gap_coefficient( const real_t& p_gap_coefficient )
    {
        s_gap_coefficient = p_gap_coefficient;
        return;
    }
    void rf_rtf_event_consumer::cluster::set_gap_power( const real_t& p_gap_power )
    {
        s_gap_power = p_gap_power;
        return;
    }
    void rf_rtf_event_consumer::cluster::set_id( const count_t& p_id )
    {
        s_id = p_id;
        return;
    }

    real_t* rf_rtf_event_consumer::cluster::s_time = NULL;
    real_t* rf_rtf_event_consumer::cluster::s_ratio = NULL;
    real_t rf_rtf_event_consumer::cluster::s_interval = 1.;
    count_t rf_rtf_event_consumer::cluster::s_min_index = 0;
    count_t rf_rtf_event_consumer::cluster::s_max_index = 0;
    real_t rf_rtf_event_consumer::cluster::s_window = 1.;
    real_t rf_rtf_event_consumer::cluster::s_add_coefficient = 1.;
    real_t rf_rtf_event_consumer::cluster::s_add_power = 0.;
    real_t rf_rtf_event_consumer::cluster::s_gap_coefficient = 2.;
    real_t rf_rtf_event_consumer::cluster::s_gap_power = 0.;
    count_t rf_rtf_event_consumer::cluster::s_id = 0;

    rf_rtf_event_consumer::cluster::cluster( const real_t& p_time, const real_t& p_frequency ) :
            f_id( s_id++ ),
            f_time( *s_time ),
            f_duration( 0. ),
            f_frequency( p_frequency ),
            f_slope( 0. ),
            f_correlation( 0. ),
            f_deviation( 0. ),
            f_occupation( 0. ),
            f_score( 0. ),
            f_times(),
            f_frequencies(),
            f_ratios(),
            f_scores(),
            f_gaps(),
            f_r_sum( 0. ),
            f_rt_sum( 0. ),
            f_rf_sum( 0. ),
            f_rtt_sum( 0. ),
            f_rff_sum( 0. ),
            f_rtf_sum( 0. ),
            f_add_occupation_sum( 0. ),
            f_add_score_sum( 0. ),
            f_gap_occupation_sum( 0. ),
            f_gap_score_sum( 0. ),
            f_gap_occupation_current( 0. ),
            f_gap_score_current( 0. ),
            f_state( e_in_add )
    {
        //msg_warning( coremsg, "cluster <" << f_id << "> created" << eom );

        register count_t t_index;
        register real_t t_point_frequency;
        register real_t t_point_ratio;

        register real_t t_init_frequency = f_frequency;
        register real_t t_init_min = t_init_frequency - s_window;
        register real_t t_init_max = t_init_frequency + s_window;
        register real_t t_init_dev_stat;
        register real_t t_init_deviation = 0.;

        pair< count_t, count_t > t_current_indices( (count_t) (floor( t_init_min / s_interval )), (count_t) (ceil( t_init_max / s_interval )) );
        if( t_current_indices.first < s_min_index )
        {
            t_current_indices.first = s_min_index;
        }
        if( t_current_indices.second > s_max_index )
        {
            t_current_indices.second = s_max_index;
        }

        pair< count_t, count_t > t_next_indices;

        register real_t t_loop_r_sum;
        register real_t t_loop_rf_sum;
        register real_t t_loop_rff_sum;
        while( true )
        {
            //msg_warning( coremsg, "  initializing:" << eom );
            //msg_warning( coremsg, "    init frequency is <" << t_init_frequency << ">" << eom );
            //msg_warning( coremsg, "    init min is <" << t_init_min << ">" << eom );
            //msg_warning( coremsg, "    init max is <" << t_init_max << ">" << eom );
            //msg_warning( coremsg, "    init deviation is <" << t_init_deviation << ">" << eom );

            t_loop_r_sum = 0.;
            t_loop_rf_sum = 0.;
            t_loop_rff_sum = 0.;
            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                t_point_frequency = t_index * s_interval;
                if( t_point_frequency < t_init_min )
                {
                    continue;
                }
                if( t_point_frequency > t_init_max )
                {
                    continue;
                }

                t_point_ratio = s_ratio[ t_index ];
                if( t_point_ratio < 0. )
                {
                    continue;
                }

                t_loop_r_sum += t_point_ratio;
                t_loop_rf_sum += t_point_ratio * t_point_frequency;
                t_loop_rff_sum += t_point_ratio * t_point_frequency * t_point_frequency;
            }

            t_init_frequency = t_loop_rf_sum / t_loop_r_sum;
            t_init_min = t_init_frequency - s_window;
            t_init_max = t_init_frequency + s_window;

            t_init_dev_stat = t_loop_rff_sum;
            t_init_dev_stat += t_init_frequency * t_init_frequency * t_loop_r_sum;
            t_init_dev_stat -= 2. * t_init_frequency * t_loop_rf_sum;
            t_init_deviation = sqrt( fabs( t_init_dev_stat ) / t_loop_r_sum );

            t_next_indices.first = (count_t) (floor( t_init_min / s_interval ));
            if( t_next_indices.first < s_min_index )
            {
                t_next_indices.first = s_min_index;
            }
            t_next_indices.second = (count_t) (ceil( t_init_max / s_interval ));
            if( t_next_indices.second > s_max_index )
            {
                t_next_indices.second = s_max_index;
            }

            if( t_current_indices == t_next_indices )
            {
                break;
            }
            else
            {
                t_current_indices = t_next_indices;
            }
        }

        f_frequency = t_init_frequency;
        f_deviation = t_init_deviation;
        f_r_sum = t_loop_r_sum;
        f_rf_sum = t_loop_rf_sum;
        f_rff_sum = t_loop_rff_sum;

        for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
        {
            t_point_ratio = s_ratio[ t_index ];
            t_point_frequency = t_index * s_interval;
            s_ratio[ t_index ] = -1.;

            if( t_point_frequency < t_init_min )
            {
                continue;
            }
            if( t_point_frequency > t_init_max )
            {
                continue;
            }
            if( t_point_ratio < 0. )
            {
                continue;
            }

            f_add_occupation_sum += 1.;
            f_add_score_sum += s_add_coefficient * pow( t_point_ratio + 1., s_add_power ) * weight( t_point_frequency - f_frequency );

            f_times.push_back( f_time );
            f_frequencies.push_back( t_point_frequency );
            f_ratios.push_back( t_point_ratio );
        }

        f_occupation = f_add_occupation_sum;
        f_score = f_add_score_sum;

        for( t_index = f_scores.size(); t_index < f_times.size(); t_index++ )
        {
            f_scores.push_back( f_score );
        }

        //msg_warning( coremsg, "  time is <" << f_time << ">" << eom );
        //msg_warning( coremsg, "  frequency is <" << f_frequency << ">" << eom );
        //msg_warning( coremsg, "  slope is <" << f_slope << ">" << eom );
        //msg_warning( coremsg, "  correlation is <" << f_correlation << ">" << eom );
        //msg_warning( coremsg, "  deviation is <" << f_deviation << ">" << eom );
        //msg_warning( coremsg, "  occupation is <" << f_occupation << ">" << eom );
        //msg_warning( coremsg, "  score is <" << f_score << ">" << eom );
    }
    rf_rtf_event_consumer::cluster::~cluster()
    {
        //msg_warning( coremsg, "cluster <" << f_id << "> destroyed" << eom );
    }

    void rf_rtf_event_consumer::cluster::update()
    {
        //msg_warning( coremsg, "cluster <" << f_id << "> updating:" << eom );

        register count_t t_index;
        register real_t t_point_time = *s_time;
        register real_t t_point_duration = t_point_time - f_time;
        register real_t t_point_frequency;
        register real_t t_point_ratio;

        register real_t t_update_tt_stat;
        register real_t t_update_ff_stat;
        register real_t t_update_tf_stat;
        register real_t t_update_ttf_stat;
        register real_t t_update_dev_stat;
        register real_t t_update_frequency = f_frequency;
        register real_t t_update_slope = f_slope;
        register real_t t_update_correlation = f_correlation;
        register real_t t_update_deviation = f_deviation;
        register real_t t_update_center = t_update_frequency + t_update_slope * t_point_duration;
        register real_t t_update_min = t_update_center - s_window;
        register real_t t_update_max = t_update_center + s_window;

        pair< count_t, count_t > t_current_indices( (count_t) (floor( t_update_min / s_interval )), (count_t) (ceil( t_update_max / s_interval )) );
        if( t_current_indices.first < s_min_index )
        {
            t_current_indices.first = s_min_index;
        }
        if( t_current_indices.second > s_max_index )
        {
            t_current_indices.second = s_max_index;
        }
        pair< count_t, count_t > t_next_indices;

        register bool_t t_loop_add;
        register real_t t_loop_r_sum;
        register real_t t_loop_rt_sum;
        register real_t t_loop_rf_sum;
        register real_t t_loop_rtt_sum;
        register real_t t_loop_rff_sum;
        register real_t t_loop_rtf_sum;
        while( true )
        {
            //msg_warning( coremsg, "  updating:" << eom );
            //msg_warning( coremsg, "    update center is <" << t_update_center << ">" << eom );
            //msg_warning( coremsg, "    update min is <" << t_update_min << ">" << eom );
            //msg_warning( coremsg, "    update max is <" << t_update_max << ">" << eom );

            t_loop_add = false;
            t_loop_r_sum = f_r_sum;
            t_loop_rt_sum = f_rt_sum;
            t_loop_rf_sum = f_rf_sum;
            t_loop_rtt_sum = f_rtt_sum;
            t_loop_rff_sum = f_rff_sum;
            t_loop_rtf_sum = f_rtf_sum;
            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                t_point_frequency = t_index * s_interval;
                if( t_point_frequency < t_update_min )
                {
                    continue;
                }
                if( t_point_frequency > t_update_max )
                {
                    continue;
                }

                t_point_ratio = s_ratio[ t_index ];
                if( t_point_ratio < 0. )
                {
                    continue;
                }

                t_loop_add = true;
                t_loop_r_sum += t_point_ratio;
                t_loop_rt_sum += t_point_ratio * t_point_duration;
                t_loop_rf_sum += t_point_ratio * t_point_frequency;
                t_loop_rtt_sum += t_point_ratio * t_point_duration * t_point_duration;
                t_loop_rff_sum += t_point_ratio * t_point_frequency * t_point_frequency;
                t_loop_rtf_sum += t_point_ratio * t_point_duration * t_point_frequency;
            }

            if( t_loop_add == false )
            {
                break;
            }

            t_update_tt_stat = t_loop_r_sum * t_loop_rtt_sum - t_loop_rt_sum * t_loop_rt_sum;
            t_update_ff_stat = t_loop_r_sum * t_loop_rff_sum - t_loop_rf_sum * t_loop_rf_sum;
            t_update_tf_stat = t_loop_r_sum * t_loop_rtf_sum - t_loop_rt_sum * t_loop_rf_sum;
            t_update_ttf_stat = t_loop_rtt_sum * t_loop_rf_sum - t_loop_rt_sum * t_loop_rtf_sum;

            t_update_frequency = t_update_ttf_stat / t_update_tt_stat;
            t_update_slope = t_update_tf_stat / t_update_tt_stat;

            t_update_dev_stat = t_loop_rff_sum;
            t_update_dev_stat += t_update_slope * t_update_slope * t_loop_rtt_sum;
            t_update_dev_stat += t_update_frequency * t_update_frequency * t_loop_r_sum;
            t_update_dev_stat -= 2. * t_update_slope * t_loop_rtf_sum;
            t_update_dev_stat -= 2. * t_update_frequency * t_loop_rf_sum;
            t_update_dev_stat += 2. * t_update_slope * t_update_frequency * t_loop_rt_sum;

            t_update_correlation = sqrt( (t_update_tf_stat * t_update_tf_stat) / (t_update_tt_stat * t_update_ff_stat) );
            t_update_deviation = sqrt( t_update_dev_stat / t_loop_r_sum );

            t_update_center = t_update_frequency + t_update_slope * t_point_duration;
            t_update_min = t_update_center - s_window;
            t_update_max = t_update_center + s_window;

            t_next_indices.first = (count_t) (floor( t_update_min / s_interval ));
            if( t_next_indices.first < s_min_index )
            {
                t_next_indices.first = s_min_index;
            }

            t_next_indices.second = (count_t) (ceil( t_update_max / s_interval ));
            if( t_next_indices.second > s_max_index )
            {
                t_next_indices.second = s_max_index;
            }

            if( t_current_indices == t_next_indices )
            {
                break;
            }
            else
            {
                t_current_indices = t_next_indices;
            }
        }

        if( t_loop_add == true )
        {
            if( f_state == e_in_add )
            {
                f_state = e_in_add;
            }
            else if( f_state == e_to_add )
            {
                f_state = e_in_add;
            }
            else if( f_state == e_in_gap )
            {
                f_state = e_to_add;
            }
            else if( f_state == e_to_gap )
            {
                f_state = e_to_add;
            }
        }
        else
        {
            if( f_state == e_in_add )
            {
                f_state = e_to_gap;
            }
            else if( f_state == e_to_add )
            {
                f_state = e_to_gap;
            }
            else if( f_state == e_in_gap )
            {
                f_state = e_in_gap;
            }
            else if( f_state == e_to_gap )
            {
                f_state = e_in_gap;
            }
        }

        if( f_state == e_in_add )
        {
            //msg_warning( coremsg, "  remained in command <add>" << eom );

            //msg_warning( coremsg, "  updating properties:" << eom );
            f_duration = t_point_duration;
            f_frequency = t_update_frequency;
            f_slope = t_update_slope;
            f_correlation = t_update_correlation;
            f_deviation = t_update_deviation;
            f_r_sum = t_loop_r_sum;
            f_rt_sum = t_loop_rt_sum;
            f_rf_sum = t_loop_rf_sum;
            f_rtt_sum = t_loop_rtt_sum;
            f_rff_sum = t_loop_rff_sum;
            f_rtt_sum = t_loop_rtt_sum;
            f_rtf_sum = t_loop_rtf_sum;
            //msg_warning( coremsg, "    duration is <" << f_duration << ">" << eom );
            //msg_warning( coremsg, "    frequency is <" << f_frequency << ">" << eom );
            //msg_warning( coremsg, "    slope is <" << f_slope << ">" << eom );
            //msg_warning( coremsg, "    correlation is <" << f_correlation << ">" << eom );
            //msg_warning( coremsg, "    deviation is <" << f_deviation << ">" << eom );

            //msg_warning( coremsg, "  updating scores:" << eom );
            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                t_point_frequency = t_index * s_interval;
                t_point_ratio = s_ratio[ t_index ];
                s_ratio[ t_index ] = -1.;

                if( t_point_frequency < t_update_min )
                {
                    continue;
                }
                if( t_point_frequency > t_update_max )
                {
                    continue;
                }
                if( t_point_ratio < 0. )
                {
                    continue;
                }

                f_add_occupation_sum += 1.;
                f_add_score_sum += s_add_coefficient * pow( t_point_ratio + 1., s_add_power ) * weight( t_point_frequency - t_update_center );

                f_times.push_back( t_point_time );
                f_frequencies.push_back( t_point_frequency );
                f_ratios.push_back( t_point_ratio );
            }

            f_occupation = f_add_occupation_sum - f_gap_occupation_sum;

            f_score = f_add_score_sum - f_gap_score_sum;

            for( t_index = f_scores.size(); t_index < f_times.size(); t_index++ )
            {
                f_scores.push_back( f_score );
            }
            //msg_warning( coremsg, "    occupation is <" << f_occupation << ">" << eom );
            //msg_warning( coremsg, "    score is <" << f_score << ">" << eom );
        }
        else if( f_state == e_to_add )
        {
            //msg_warning( coremsg, "  switched to command <add>" << eom );

            //msg_warning( coremsg, "  updating properties:" << eom );
            f_duration = t_point_duration;
            f_frequency = t_update_frequency;
            f_slope = t_update_slope;
            f_correlation = t_update_correlation;
            f_deviation = t_update_deviation;
            f_r_sum = t_loop_r_sum;
            f_rt_sum = t_loop_rt_sum;
            f_rf_sum = t_loop_rf_sum;
            f_rtt_sum = t_loop_rtt_sum;
            f_rff_sum = t_loop_rff_sum;
            f_rtt_sum = t_loop_rtt_sum;
            f_rtf_sum = t_loop_rtf_sum;
            //msg_warning( coremsg, "    duration is <" << f_duration << ">" << eom );
            //msg_warning( coremsg, "    frequency is <" << f_frequency << ">" << eom );
            //msg_warning( coremsg, "    slope is <" << f_slope << ">" << eom );
            //msg_warning( coremsg, "    correlation is <" << f_correlation << ">" << eom );
            //msg_warning( coremsg, "    deviation is <" << f_deviation << ">" << eom );

            //msg_warning( coremsg, " updating scores:" << eom );
            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                t_point_frequency = t_index * s_interval;
                t_point_ratio = s_ratio[ t_index ];
                s_ratio[ t_index ] = -1.;

                if( t_point_frequency < t_update_min )
                {
                    continue;
                }
                if( t_point_frequency > t_update_max )
                {
                    continue;
                }
                if( t_point_ratio < 0. )
                {
                    continue;
                }

                f_add_occupation_sum += 1.;
                f_add_score_sum += s_add_coefficient * pow( t_point_ratio + 1., s_add_power ) * weight( t_point_frequency - t_update_center );

                f_times.push_back( t_point_time );
                f_frequencies.push_back( t_point_frequency );
                f_ratios.push_back( t_point_ratio );
            }

            f_gap_occupation_sum += f_gap_occupation_current;
            f_gap_occupation_current = 0.;
            f_occupation = f_add_occupation_sum - f_gap_occupation_sum;

            f_gap_score_sum += f_gap_score_current;
            f_gap_score_current = 0.;
            f_score = f_add_score_sum - f_gap_score_sum;

            for( t_index = f_scores.size(); t_index < f_times.size(); t_index++ )
            {
                f_scores.push_back( f_score );
            }
            //msg_warning( coremsg, "    occupation is <" << f_occupation << ">" << eom );
            //msg_warning( coremsg, "    score is <" << f_score << ">" << eom );
        }
        else if( f_state == e_in_gap )
        {
            //msg_warning( coremsg, "  remained in command <gap>" << eom );

            //msg_warning( coremsg, "  updating scores:" << eom );
            f_gaps.back() += 1.;

            f_gap_occupation_current += 1.;
            f_occupation = f_add_occupation_sum - f_gap_occupation_sum - f_gap_occupation_current;

            f_gap_score_current = s_gap_coefficient * pow( f_gap_occupation_current, s_gap_power );
            f_score = f_add_score_sum - f_gap_score_sum - f_gap_score_current;
            //msg_warning( coremsg, "    occupation is <" << f_occupation << ">" << eom );
            //msg_warning( coremsg, "    score is <" << f_score << ">" << eom );
        }
        else if( f_state == e_to_gap )
        {
            //msg_warning( coremsg, "  switched to command <gap>" << eom );

            //msg_warning( coremsg, "  updating scores:" << eom );
            f_gaps.push_back( 1. );

            f_gap_occupation_current = 1.;
            f_occupation = f_add_occupation_sum - f_gap_occupation_sum - f_gap_occupation_current;

            f_gap_score_current = s_gap_coefficient * pow( f_gap_occupation_current, s_gap_power );
            f_score = f_add_score_sum - f_gap_score_sum - f_gap_score_current;
            //msg_warning( coremsg, "    occupation is <" << f_occupation << ">" << eom );
            //msg_warning( coremsg, "    score is <" << f_score << ">" << eom );
        }

        return;
    }

    const count_t& rf_rtf_event_consumer::cluster::id() const
    {
        return f_id;
    }
    const real_t& rf_rtf_event_consumer::cluster::time() const
    {
        return f_time;
    }
    const real_t& rf_rtf_event_consumer::cluster::duration() const
    {
        return f_duration;
    }
    const real_t& rf_rtf_event_consumer::cluster::frequency() const
    {
        return f_frequency;
    }
    const real_t& rf_rtf_event_consumer::cluster::slope() const
    {
        return f_slope;
    }
    const real_t& rf_rtf_event_consumer::cluster::correlation() const
    {
        return f_correlation;
    }
    const real_t& rf_rtf_event_consumer::cluster::deviation() const
    {
        return f_deviation;
    }
    const real_t& rf_rtf_event_consumer::cluster::occupation() const
    {
        return f_occupation;
    }
    const real_t& rf_rtf_event_consumer::cluster::score() const
    {
        return f_score;
    }

    const vector< real_t >& rf_rtf_event_consumer::cluster::times() const
    {
        return f_times;
    }
    const vector< real_t >& rf_rtf_event_consumer::cluster::frequencies() const
    {
        return f_frequencies;
    }
    const vector< real_t >& rf_rtf_event_consumer::cluster::ratios() const
    {
        return f_ratios;
    }
    const vector< real_t >& rf_rtf_event_consumer::cluster::scores() const
    {
        return f_scores;
    }
    const vector< real_t >& rf_rtf_event_consumer::cluster::gaps() const
    {
        return f_gaps;
    }

    real_t rf_rtf_event_consumer::cluster::weight( const real_t& p_frequency ) const
    {
        if( p_frequency > 1. * s_window )
        {
            return 0.;
        }
        if( p_frequency < -1. * s_window )
        {
            return 0.;
        }
        return (.5 + .5 * cos( M_PI * p_frequency / s_window ));
    }

    //****
    //line
    //****

    void rf_rtf_event_consumer::line::set_time( real_t* p_time )
    {
        s_time = p_time;
        return;
    }
    void rf_rtf_event_consumer::line::set_ratio( real_t* p_ratio )
    {
        s_ratio = p_ratio;
        return;
    }
    void rf_rtf_event_consumer::line::set_interval( const real_t& p_interval )
    {
        s_interval = p_interval;
        return;
    }
    void rf_rtf_event_consumer::line::set_min_index( const count_t& p_min_index )
    {
        s_min_index = p_min_index;
        return;
    }
    void rf_rtf_event_consumer::line::set_max_index( const count_t& p_max_index )
    {
        s_max_index = p_max_index;
        return;
    }
    void rf_rtf_event_consumer::line::set_tolerance( const real_t& p_tolerance )
    {
        s_tolerance = p_tolerance;
        return;
    }
    void rf_rtf_event_consumer::line::set_add_coefficient( const real_t& p_add_coefficient )
    {
        s_add_coefficient = p_add_coefficient;
        return;
    }
    void rf_rtf_event_consumer::line::set_add_power( const real_t& p_add_power )
    {
        s_add_power = p_add_power;
        return;
    }
    void rf_rtf_event_consumer::line::set_gap_coefficient( const real_t& p_gap_coefficient )
    {
        s_gap_coefficient = p_gap_coefficient;
        return;
    }
    void rf_rtf_event_consumer::line::set_gap_power( const real_t& p_gap_power )
    {
        s_gap_power = p_gap_power;
        return;
    }
    void rf_rtf_event_consumer::line::set_id( const count_t& p_id )
    {
        s_id = p_id;
        return;
    }

    real_t* rf_rtf_event_consumer::line::s_time = NULL;
    real_t* rf_rtf_event_consumer::line::s_ratio = NULL;
    real_t rf_rtf_event_consumer::line::s_interval = 1.;
    count_t rf_rtf_event_consumer::line::s_min_index = 0;
    count_t rf_rtf_event_consumer::line::s_max_index = 0;
    real_t rf_rtf_event_consumer::line::s_tolerance = 1.;
    real_t rf_rtf_event_consumer::line::s_add_coefficient = 1.;
    real_t rf_rtf_event_consumer::line::s_add_power = 0.;
    real_t rf_rtf_event_consumer::line::s_gap_coefficient = 2.;
    real_t rf_rtf_event_consumer::line::s_gap_power = 0.;
    count_t rf_rtf_event_consumer::line::s_id = 0;

    rf_rtf_event_consumer::line::line( const cluster& p_cluster ) :
            f_id( s_id++ ),
            f_time( p_cluster.time() ),
            f_duration( p_cluster.duration() ),
            f_frequency( p_cluster.frequency() ),
            f_slope( p_cluster.slope() ),
            f_correlation( p_cluster.correlation() ),
            f_deviation( p_cluster.deviation() ),
            f_occupation( 0. ),
            f_score( 0. ),
            f_times(),
            f_frequencies(),
            f_ratios(),
            f_scores(),
            f_gaps( p_cluster.gaps() ),
            f_r_sum( 0. ),
            f_rt_sum( 0. ),
            f_rf_sum( 0. ),
            f_rtt_sum( 0. ),
            f_rff_sum( 0. ),
            f_rtf_sum( 0. ),
            f_add_occupation_sum( 0. ),
            f_add_score_sum( 0. ),
            f_gap_occupation_sum( 0. ),
            f_gap_score_sum( 0. ),
            f_gap_occupation_current( 0. ),
            f_gap_score_current( 0. ),
            f_state( e_in_add )
    {
        //msg_warning( coremsg, "line <" << f_id << "> created" << eom );

        register count_t t_index;
        register real_t t_point_time;
        register real_t t_point_duration;
        register real_t t_point_frequency;
        register real_t t_point_ratio;
        register real_t t_point_center;
        register real_t t_point_max;
        register real_t t_point_min;

        register real_t t_init_tt_stat;
        register real_t t_init_ff_stat;
        register real_t t_init_tf_stat;
        register real_t t_init_ttf_stat;
        register real_t t_init_dev_stat;
        register real_t t_init_time = f_time;
        register real_t t_init_duration = f_duration;
        register real_t t_init_frequency = f_frequency;
        register real_t t_init_slope = f_slope;
        register real_t t_init_correlation = f_correlation;
        register real_t t_init_deviation = f_deviation;

        set< count_t > t_current_indices;
        for( t_index = 0; t_index < p_cluster.times().size(); t_index++ )
        {
            t_current_indices.insert( t_index );
        }

        set< count_t > t_next_indices;

        register real_t t_loop_t_min;
        register real_t t_loop_t_max;
        register real_t t_loop_r_sum;
        register real_t t_loop_rt_sum;
        register real_t t_loop_rf_sum;
        register real_t t_loop_rtt_sum;
        register real_t t_loop_rff_sum;
        register real_t t_loop_rtf_sum;
        while( true )
        {
            //msg_warning( coremsg, "  initializing:" << eom );
            //msg_warning( coremsg, "    time is <" << t_init_time << ">" << eom );
            //msg_warning( coremsg, "    frequency is <" << t_init_frequency << ">" << eom );
            //msg_warning( coremsg, "    slope is <" << t_init_slope << ">" << eom );

            t_loop_t_min = numeric_limits< real_t >::max();
            t_loop_t_max = numeric_limits< real_t >::min();
            t_loop_r_sum = 0.;
            t_loop_rt_sum = 0.;
            t_loop_rf_sum = 0.;
            t_loop_rtt_sum = 0.;
            t_loop_rff_sum = 0.;
            t_loop_rtf_sum = 0.;
            t_next_indices.clear();
            for( t_index = 0; t_index < p_cluster.times().size(); t_index++ )
            {
                t_point_time = p_cluster.times().at( t_index );
                t_point_duration = t_point_time - t_init_time;
                t_point_frequency = p_cluster.frequencies().at( t_index );
                t_point_ratio = p_cluster.ratios().at( t_index );
                t_point_center = t_init_frequency + t_init_slope * t_point_duration;
                t_point_max = t_point_center + s_tolerance * t_init_deviation;
                t_point_min = t_point_center - s_tolerance * t_init_deviation;

                if( t_point_frequency > t_point_max )
                {
                    continue;
                }

                if( t_point_frequency < t_point_min )
                {
                    continue;
                }

                t_loop_t_min = t_loop_t_min < t_point_time ? t_loop_t_min : t_point_time;
                t_loop_t_max = t_loop_t_max > t_point_time ? t_loop_t_max : t_point_time;
                t_loop_r_sum += t_point_ratio;
                t_loop_rt_sum += t_point_ratio * t_point_duration;
                t_loop_rf_sum += t_point_ratio * t_point_frequency;
                t_loop_rtt_sum += t_point_ratio * t_point_duration * t_point_duration;
                t_loop_rff_sum += t_point_ratio * t_point_frequency * t_point_frequency;
                t_loop_rtf_sum += t_point_ratio * t_point_duration * t_point_frequency;

                t_next_indices.insert( t_index );
            }

            if( t_next_indices.size() == 0 )
            {
                break;
            }

            t_init_time = t_loop_t_min;
            t_init_duration = t_loop_t_max - t_loop_t_min;

            t_init_tt_stat = t_loop_r_sum * t_loop_rtt_sum - t_loop_rt_sum * t_loop_rt_sum;
            t_init_ff_stat = t_loop_r_sum * t_loop_rff_sum - t_loop_rf_sum * t_loop_rf_sum;
            t_init_tf_stat = t_loop_r_sum * t_loop_rtf_sum - t_loop_rt_sum * t_loop_rf_sum;
            t_init_ttf_stat = t_loop_rtt_sum * t_loop_rf_sum - t_loop_rt_sum * t_loop_rtf_sum;

            t_init_frequency = t_init_ttf_stat / t_init_tt_stat;
            t_init_slope = t_init_tf_stat / t_init_tt_stat;

            t_init_dev_stat = t_loop_rff_sum;
            t_init_dev_stat += t_init_slope * t_init_slope * t_loop_rtt_sum;
            t_init_dev_stat += t_init_frequency * t_init_frequency * t_loop_r_sum;
            t_init_dev_stat -= 2. * t_init_slope * t_loop_rtf_sum;
            t_init_dev_stat -= 2. * t_init_frequency * t_loop_rf_sum;
            t_init_dev_stat += 2. * t_init_slope * t_init_frequency * t_loop_rt_sum;

            t_init_correlation = sqrt( (t_init_tf_stat * t_init_tf_stat) / (t_init_tt_stat * t_init_ff_stat) );
            t_init_deviation = sqrt( t_init_dev_stat / t_loop_r_sum );

            set< count_t >::iterator t_it = t_current_indices.begin();
            while( t_it != t_current_indices.end() )
            {
                if( t_next_indices.find( *t_it ) == t_next_indices.end() )
                {
                    break;
                }
                t_it++;
            }

            if( t_it == t_current_indices.end() )
            {
                break;
            }
            else
            {
                t_current_indices = t_next_indices;
            }
        }

        f_time = t_init_time;
        f_duration = t_init_duration;
        f_frequency = t_init_frequency;
        f_slope = t_init_slope;
        f_correlation = t_init_correlation;
        f_deviation = t_init_deviation;
        f_r_sum = t_loop_r_sum;
        f_rt_sum = t_loop_rt_sum;
        f_rf_sum = t_loop_rf_sum;
        f_rtt_sum = t_loop_rtt_sum;
        f_rff_sum = t_loop_rff_sum;
        f_rtt_sum = t_loop_rtt_sum;
        f_rtf_sum = t_loop_rtf_sum;

        for( set< count_t >::iterator t_it = t_current_indices.begin(); t_it != t_current_indices.end(); t_it++ )
        {
            t_index = *t_it;
            t_point_time = p_cluster.times().at( t_index );
            t_point_duration = t_point_time - f_time;
            t_point_frequency = p_cluster.frequencies().at( t_index );
            t_point_ratio = p_cluster.ratios().at( t_index );
            t_point_center = f_frequency + f_slope * t_point_duration;

            f_add_occupation_sum += 1.;
            f_add_score_sum += s_add_coefficient * pow( t_point_ratio + 1., s_add_power ) * weight( t_point_frequency - t_point_center );

            f_times.push_back( t_point_time );
            f_frequencies.push_back( t_point_frequency );
            f_ratios.push_back( t_point_ratio );
        }

        for( t_index = 0; t_index < p_cluster.gaps().size(); t_index++ )
        {
            f_gap_occupation_sum += 1.;
            f_gap_score_sum += s_gap_coefficient * pow( p_cluster.gaps().at( t_index ), s_gap_power );
        }

        f_occupation = f_add_occupation_sum - f_gap_occupation_sum;
        f_score = f_add_score_sum - f_gap_score_sum;

        for( t_index = f_scores.size(); t_index < f_times.size(); t_index++ )
        {
            f_scores.push_back( f_score );
        }

        //msg_warning( coremsg, "  time is <" << f_time << ">" << eom );
        //msg_warning( coremsg, "  frequency is <" << f_frequency << ">" << eom );
        //msg_warning( coremsg, "  slope is <" << f_slope << ">" << eom );
        //msg_warning( coremsg, "  correlation is <" << f_correlation << ">" << eom );
        //msg_warning( coremsg, "  deviation is <" << f_deviation << ">" << eom );
        //msg_warning( coremsg, "  occupation is <" << f_occupation << ">" << eom );
        //msg_warning( coremsg, "  score is <" << f_score << ">" << eom );
    }
    rf_rtf_event_consumer::line::~line()
    {
        //msg_warning( coremsg, "line <" << f_id << "> destroyed" << eom );
    }

    void rf_rtf_event_consumer::line::update()
    {
        //msg_warning( coremsg, "line <" << f_id << "> updating:" << eom );

        register count_t t_index;
        register real_t t_point_time = *s_time;
        register real_t t_point_duration = t_point_time - f_time;
        register real_t t_point_frequency;
        register real_t t_point_ratio;

        register real_t t_update_tt_stat;
        register real_t t_update_ff_stat;
        register real_t t_update_tf_stat;
        register real_t t_update_ttf_stat;
        register real_t t_update_dev_stat;
        register real_t t_update_frequency = f_frequency;
        register real_t t_update_slope = f_slope;
        register real_t t_update_correlation = f_correlation;
        register real_t t_update_deviation = f_deviation;
        register real_t t_update_center = t_update_frequency + t_update_slope * t_point_duration;
        register real_t t_update_min = t_update_center - s_tolerance * f_deviation;
        register real_t t_update_max = t_update_center + s_tolerance * f_deviation;

        pair< count_t, count_t > t_current_indices( (count_t) (floor( t_update_min / s_interval )), (count_t) (ceil( t_update_max / s_interval )) );
        if( t_current_indices.first < s_min_index )
        {
            t_current_indices.first = s_min_index;
        }
        if( t_current_indices.second > s_max_index )
        {
            t_current_indices.second = s_max_index;
        }
        pair< count_t, count_t > t_next_indices;

        register bool_t t_loop_add;
        register real_t t_loop_r_sum;
        register real_t t_loop_rt_sum;
        register real_t t_loop_rf_sum;
        register real_t t_loop_rtt_sum;
        register real_t t_loop_rff_sum;
        register real_t t_loop_rtf_sum;
        while( true )
        {
            //msg_warning( coremsg, "  updating:" << eom );
            //msg_warning( coremsg, "    update center is <" << t_update_center << ">" << eom );
            //msg_warning( coremsg, "    update min is <" << t_update_min << ">" << eom );
            //msg_warning( coremsg, "    update max is <" << t_update_max << ">" << eom );

            t_loop_add = false;
            t_loop_r_sum = f_r_sum;
            t_loop_rt_sum = f_rt_sum;
            t_loop_rf_sum = f_rf_sum;
            t_loop_rtt_sum = f_rtt_sum;
            t_loop_rff_sum = f_rff_sum;
            t_loop_rtf_sum = f_rtf_sum;
            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                t_point_frequency = t_index * s_interval;
                if( t_point_frequency < t_update_min )
                {
                    continue;
                }
                if( t_point_frequency > t_update_max )
                {
                    continue;
                }

                t_point_ratio = s_ratio[ t_index ];
                if( t_point_ratio < 0. )
                {
                    continue;
                }

                t_loop_add = true;
                t_loop_r_sum += t_point_ratio;
                t_loop_rt_sum += t_point_ratio * t_point_duration;
                t_loop_rf_sum += t_point_ratio * t_point_frequency;
                t_loop_rtt_sum += t_point_ratio * t_point_duration * t_point_duration;
                t_loop_rff_sum += t_point_ratio * t_point_frequency * t_point_frequency;
                t_loop_rtf_sum += t_point_ratio * t_point_duration * t_point_frequency;
            }

            if( t_loop_add == false )
            {
                break;
            }

            t_update_tt_stat = t_loop_r_sum * t_loop_rtt_sum - t_loop_rt_sum * t_loop_rt_sum;
            t_update_ff_stat = t_loop_r_sum * t_loop_rff_sum - t_loop_rf_sum * t_loop_rf_sum;
            t_update_tf_stat = t_loop_r_sum * t_loop_rtf_sum - t_loop_rt_sum * t_loop_rf_sum;
            t_update_ttf_stat = t_loop_rtt_sum * t_loop_rf_sum - t_loop_rt_sum * t_loop_rtf_sum;

            t_update_frequency = t_update_ttf_stat / t_update_tt_stat;
            t_update_slope = t_update_tf_stat / t_update_tt_stat;

            t_update_dev_stat = t_loop_rff_sum;
            t_update_dev_stat += t_update_slope * t_update_slope * t_loop_rtt_sum;
            t_update_dev_stat += t_update_frequency * t_update_frequency * t_loop_r_sum;
            t_update_dev_stat -= 2. * t_update_slope * t_loop_rtf_sum;
            t_update_dev_stat -= 2. * t_update_frequency * t_loop_rf_sum;
            t_update_dev_stat += 2. * t_update_slope * t_update_frequency * t_loop_rt_sum;

            t_update_correlation = sqrt( (t_update_tf_stat * t_update_tf_stat) / (t_update_tt_stat * t_update_ff_stat) );
            t_update_deviation = sqrt( t_update_dev_stat / t_loop_r_sum );

            t_update_center = t_update_frequency + t_update_slope * t_point_duration;
            t_update_min = t_update_center - s_tolerance * f_deviation;
            t_update_max = t_update_center + s_tolerance * f_deviation;

            t_next_indices.first = (count_t) (floor( t_update_min / s_interval ));
            if( t_next_indices.first < s_min_index )
            {
                t_next_indices.first = s_min_index;
            }

            t_next_indices.second = (count_t) (ceil( t_update_max / s_interval ));
            if( t_next_indices.second > s_max_index )
            {
                t_next_indices.second = s_max_index;
            }

            if( t_current_indices == t_next_indices )
            {
                break;
            }
            else
            {
                t_current_indices = t_next_indices;
            }
        }

        if( t_loop_add == true )
        {
            if( f_state == e_in_add )
            {
                f_state = e_in_add;
            }
            else if( f_state == e_to_add )
            {
                f_state = e_in_add;
            }
            else if( f_state == e_in_gap )
            {
                f_state = e_to_add;
            }
            else if( f_state == e_to_gap )
            {
                f_state = e_to_add;
            }
        }
        else
        {
            if( f_state == e_in_add )
            {
                f_state = e_to_gap;
            }
            else if( f_state == e_to_add )
            {
                f_state = e_to_gap;
            }
            else if( f_state == e_in_gap )
            {
                f_state = e_in_gap;
            }
            else if( f_state == e_to_gap )
            {
                f_state = e_in_gap;
            }
        }

        if( f_state == e_in_add )
        {
            //msg_warning( coremsg, "  remained in command <add>" << eom );

            //msg_warning( coremsg, "  updating properties:" << eom );
            f_duration = t_point_duration;
            f_frequency = t_update_frequency;
            f_slope = t_update_slope;
            f_correlation = t_update_correlation;
            f_deviation = t_update_deviation;
            f_r_sum = t_loop_r_sum;
            f_rt_sum = t_loop_rt_sum;
            f_rf_sum = t_loop_rf_sum;
            f_rtt_sum = t_loop_rtt_sum;
            f_rff_sum = t_loop_rff_sum;
            f_rtt_sum = t_loop_rtt_sum;
            f_rtf_sum = t_loop_rtf_sum;
            //msg_warning( coremsg, "    duration is <" << f_duration << ">" << eom );
            //msg_warning( coremsg, "    frequency is <" << f_frequency << ">" << eom );
            //msg_warning( coremsg, "    slope is <" << f_slope << ">" << eom );
            //msg_warning( coremsg, "    correlation is <" << f_correlation << ">" << eom );
            //msg_warning( coremsg, "    deviation is <" << f_deviation << ">" << eom );

            //msg_warning( coremsg, "  updating scores:" << eom );
            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                t_point_frequency = t_index * s_interval;
                t_point_ratio = s_ratio[ t_index ];
                s_ratio[ t_index ] = -1.;

                if( t_point_frequency < t_update_min )
                {
                    continue;
                }
                if( t_point_frequency > t_update_max )
                {
                    continue;
                }
                if( t_point_ratio < 0. )
                {
                    continue;
                }

                f_add_occupation_sum += 1.;
                f_add_score_sum += s_add_coefficient * pow( t_point_ratio + 1., s_add_power ) * weight( t_point_frequency - t_update_center );

                f_times.push_back( t_point_time );
                f_frequencies.push_back( t_point_frequency );
                f_ratios.push_back( t_point_ratio );
            }

            f_occupation = f_add_occupation_sum - f_gap_occupation_sum;

            f_score = f_add_score_sum - f_gap_score_sum;

            for( t_index = f_scores.size(); t_index < f_times.size(); t_index++ )
            {
                f_scores.push_back( f_score );
            }
            //msg_warning( coremsg, "    occupation is <" << f_occupation << ">" << eom );
            //msg_warning( coremsg, "    score is <" << f_score << ">" << eom );
        }
        else if( f_state == e_to_add )
        {
            //msg_warning( coremsg, "  switched to command <add>" << eom );

            //msg_warning( coremsg, "  updating properties:" << eom );
            f_duration = t_point_duration;
            f_frequency = t_update_frequency;
            f_slope = t_update_slope;
            f_correlation = t_update_correlation;
            f_deviation = t_update_deviation;
            f_r_sum = t_loop_r_sum;
            f_rt_sum = t_loop_rt_sum;
            f_rf_sum = t_loop_rf_sum;
            f_rtt_sum = t_loop_rtt_sum;
            f_rff_sum = t_loop_rff_sum;
            f_rtt_sum = t_loop_rtt_sum;
            f_rtf_sum = t_loop_rtf_sum;
            //msg_warning( coremsg, "    duration is <" << f_duration << ">" << eom );
            //msg_warning( coremsg, "    frequency is <" << f_frequency << ">" << eom );
            //msg_warning( coremsg, "    slope is <" << f_slope << ">" << eom );
            //msg_warning( coremsg, "    correlation is <" << f_correlation << ">" << eom );
            //msg_warning( coremsg, "    deviation is <" << f_deviation << ">" << eom );

            //msg_warning( coremsg, " updating scores:" << eom );
            for( t_index = t_current_indices.first; t_index <= t_current_indices.second; t_index++ )
            {
                t_point_frequency = t_index * s_interval;
                t_point_ratio = s_ratio[ t_index ];
                s_ratio[ t_index ] = -1.;

                if( t_point_frequency < t_update_min )
                {
                    continue;
                }
                if( t_point_frequency > t_update_max )
                {
                    continue;
                }
                if( t_point_ratio < 0. )
                {
                    continue;
                }

                f_add_occupation_sum += 1.;
                f_add_score_sum += s_add_coefficient * pow( t_point_ratio + 1., s_add_power ) * weight( t_point_frequency - t_update_center );

                f_times.push_back( t_point_time );
                f_frequencies.push_back( t_point_frequency );
                f_ratios.push_back( t_point_ratio );
            }

            f_gap_occupation_sum += f_gap_occupation_current;
            f_gap_occupation_current = 0.;
            f_occupation = f_add_occupation_sum - f_gap_occupation_sum;

            f_gap_score_sum += f_gap_score_current;
            f_gap_score_current = 0.;
            f_score = f_add_score_sum - f_gap_score_sum;

            for( t_index = f_scores.size(); t_index < f_times.size(); t_index++ )
            {
                f_scores.push_back( f_score );
            }
            //msg_warning( coremsg, "    occupation is <" << f_occupation << ">" << eom );
            //msg_warning( coremsg, "    score is <" << f_score << ">" << eom );
        }
        else if( f_state == e_in_gap )
        {
            //msg_warning( coremsg, "  remained in command <gap>" << eom );

            //msg_warning( coremsg, "  updating scores:" << eom );
            f_gaps.back() += 1.;

            f_gap_occupation_current += 1.;
            f_occupation = f_add_occupation_sum - f_gap_occupation_sum - f_gap_occupation_current;

            f_gap_score_current = s_gap_coefficient * pow( f_gap_occupation_current, s_gap_power );
            f_score = f_add_score_sum - f_gap_score_sum - f_gap_score_current;
            //msg_warning( coremsg, "    occupation is <" << f_occupation << ">" << eom );
            //msg_warning( coremsg, "    score is <" << f_score << ">" << eom );
        }
        else if( f_state == e_to_gap )
        {
            //msg_warning( coremsg, "  switched to command <gap>" << eom );

            //msg_warning( coremsg, "  updating scores:" << eom );
            f_gaps.push_back( 1. );

            f_gap_occupation_current = 1.;
            f_occupation = f_add_occupation_sum - f_gap_occupation_sum - f_gap_occupation_current;

            f_gap_score_current = s_gap_coefficient * pow( f_gap_occupation_current, s_gap_power );
            f_score = f_add_score_sum - f_gap_score_sum - f_gap_score_current;
            //msg_warning( coremsg, "    occupation is <" << f_occupation << ">" << eom );
            //msg_warning( coremsg, "    score is <" << f_score << ">" << eom );
        }

        return;
    }

    const count_t& rf_rtf_event_consumer::line::id() const
    {
        return f_id;
    }
    const real_t& rf_rtf_event_consumer::line::time() const
    {
        return f_time;
    }
    const real_t& rf_rtf_event_consumer::line::duration() const
    {
        return f_duration;
    }
    const real_t& rf_rtf_event_consumer::line::frequency() const
    {
        return f_frequency;
    }
    const real_t& rf_rtf_event_consumer::line::slope() const
    {
        return f_slope;
    }
    const real_t& rf_rtf_event_consumer::line::correlation() const
    {
        return f_correlation;
    }
    const real_t& rf_rtf_event_consumer::line::deviation() const
    {
        return f_deviation;
    }
    const real_t& rf_rtf_event_consumer::line::occupation() const
    {
        return f_occupation;
    }
    const real_t& rf_rtf_event_consumer::line::score() const
    {
        return f_score;
    }

    const vector< real_t >& rf_rtf_event_consumer::line::times() const
    {
        return f_times;
    }
    const vector< real_t >& rf_rtf_event_consumer::line::frequencies() const
    {
        return f_frequencies;
    }
    const vector< real_t >& rf_rtf_event_consumer::line::ratios() const
    {
        return f_ratios;
    }
    const vector< real_t >& rf_rtf_event_consumer::line::scores() const
    {
        return f_scores;
    }
    const vector< real_t >& rf_rtf_event_consumer::line::gaps() const
    {
        return f_gaps;
    }

    real_t rf_rtf_event_consumer::line::weight( const real_t& p_frequency ) const
    {
        if( p_frequency > 1. * s_tolerance * f_deviation )
        {
            return 0.;
        }
        if( p_frequency < -1. * s_tolerance * f_deviation )
        {
            return 0.;
        }
        return (.5 + .5 * cos( M_PI * p_frequency / (s_tolerance * f_deviation) ));
    }

    //*****
    //event
    //*****

    void rf_rtf_event_consumer::event::set_id( const count_t& p_id )
    {
        s_id = p_id;
        return;
    }

    count_t rf_rtf_event_consumer::event::s_id = 0;

    rf_rtf_event_consumer::event::event( const line& p_line ) :
            f_id( s_id++ ),
            f_time( p_line.time() ),
            f_duration( p_line.duration() ),
            f_line_times( 2, 0. ),
            f_line_frequencies( 2, 0. ),
            f_data_times( p_line.times() ),
            f_data_frequencies( p_line.frequencies() ),
            f_data_ratios( p_line.ratios() )
    {
        f_line_times.at( 0 ) = p_line.time();
        f_line_times.at( 1 ) = p_line.time() + p_line.duration();
        f_line_frequencies.at( 0 ) = p_line.frequency();
        f_line_frequencies.at( 1 ) = p_line.frequency() + p_line.slope() * p_line.duration();
    }
    rf_rtf_event_consumer::event::~event()
    {
    }

    void rf_rtf_event_consumer::event::add( const line& p_line )
    {
        f_duration = p_line.time() + p_line.duration() - f_time;

        f_data_times.insert( f_data_times.end(), p_line.times().begin(), p_line.times().end() );
        f_data_frequencies.insert( f_data_frequencies.end(), p_line.frequencies().begin(), p_line.frequencies().end() );
        f_data_ratios.insert( f_data_ratios.end(), p_line.ratios().begin(), p_line.ratios().end() );

        f_line_times.push_back( p_line.time() );
        f_line_times.push_back( p_line.time() + p_line.duration() );
        f_line_frequencies.push_back( p_line.frequency() );
        f_line_frequencies.push_back( p_line.frequency() + p_line.slope() * p_line.duration() );

        return;
    }

    const count_t& rf_rtf_event_consumer::event::id() const
    {
        return f_id;
    }

    const real_t& rf_rtf_event_consumer::event::time() const
    {
        return f_time;
    }
    const real_t& rf_rtf_event_consumer::event::duration() const
    {
        return f_duration;
    }

    const vector< real_t >& rf_rtf_event_consumer::event::line_times() const
    {
        return f_line_times;
    }
    const vector< real_t >& rf_rtf_event_consumer::event::line_frequencies() const
    {
        return f_line_frequencies;
    }

    const vector< real_t >& rf_rtf_event_consumer::event::data_times() const
    {
        return f_data_times;
    }
    const vector< real_t >& rf_rtf_event_consumer::event::data_frequencies() const
    {
        return f_data_frequencies;
    }
    const vector< real_t >& rf_rtf_event_consumer::event::data_ratios() const
    {
        return f_data_ratios;
    }

}

