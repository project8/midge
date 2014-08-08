#include "rf_event_consumer.hh"

#include "plot.hh"

#include <stack>
using std::stack;

#include <cmath>

namespace midge
{

    rf_event_consumer::rf_event_consumer() :
            f_ratio_file( "" ),
            f_background_file( "" ),
            f_frequency_minimum( 0. ),
            f_frequency_maximum( 0. ),
            f_cluster_file( "" ),
            f_cluster_add_ratio( -1. ),
            f_cluster_slope( 0. ),
            f_cluster_spread( 1. ),
            f_cluster_add_coefficient( 1. ),
            f_cluster_add_power( 0. ),
            f_cluster_gap_coefficient( 2. ),
            f_cluster_gap_power( 0. ),
            f_line_file( "" ),
            f_line_start_score( 100. ),
            f_line_stop_score( 100. ),
            f_line_tolerance( 1. ),
            f_line_add_coefficient( 1. ),
            f_line_add_power( 0. ),
            f_line_gap_coefficient( 2. ),
            f_line_gap_power( 0. ),
            f_event_file( "" ),
            f_event_time_tolerance( 0. ),
            f_plot( false ),
            f_plot_ratio_key( "" ),
            f_plot_ratio_name( "" ),
            f_chart_ratio_title( "" ),
            f_axis_ratio_title( "" ),
            f_plot_cluster_key( "" ),
            f_plot_cluster_name( "" ),
            f_chart_cluster_title( "" ),
            f_axis_cluster_title( "" ),
            f_ratio_stream( NULL ),
            f_ratio_label( NULL ),
            f_ratio_tree( NULL ),
            f_cluster_stream( NULL ),
            f_cluster_label( NULL ),
            f_cluster_tree( NULL ),
            f_line_stream( NULL ),
            f_line_label( NULL ),
            f_line_tree( NULL ),
            f_event_stream( NULL ),
            f_event_label( NULL ),
            f_event_tree( NULL ),
            f_tree_data_time( 0. ),
            f_tree_data_frequency( 0. ),
            f_tree_data_ratio( 0. ),
            f_tree_id( 0 ),
            f_tree_score( 0. ),
            f_tree_time( 0. ),
            f_tree_duration( 0. ),
            f_tree_frequency( 0. ),
            f_tree_slope( 0. ),
            f_tree_correlation( 0. ),
            f_tree_deviation( 0. ),
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
    rf_event_consumer::~rf_event_consumer()
    {
    }

    bool rf_event_consumer::start_consumer()
    {
        if( f_plot == true )
        {
            plot::get_instance()->initialize();
        }

        if( f_ratio_file.size() > 0 )
        {
            f_ratio_stream = new TFile( f_ratio_file.c_str(), "RECREATE" );
            f_ratio_label = new TObjString( "midge_ratios" );
            f_ratio_tree = new TTree( "ratios", "ratios" );
            f_ratio_tree->SetDirectory( f_ratio_stream );
            f_ratio_tree->Branch( "time", &f_tree_data_time );
            f_ratio_tree->Branch( "frequency", &f_tree_data_frequency );
            f_ratio_tree->Branch( "ratio", &f_tree_data_ratio );
        }

        if( f_cluster_file.size() > 0 )
        {
            f_cluster_stream = new TFile( f_cluster_file.c_str(), "RECREATE" );
            f_cluster_label = new TObjString( "midge_clusters" );
            f_cluster_tree = new TTree( "clusters", "clusters" );
            f_cluster_tree->SetDirectory( f_cluster_stream );
            f_cluster_tree->Branch( "id", &f_tree_id );
            f_cluster_tree->Branch( "score", &f_tree_score );
            f_cluster_tree->Branch( "time", &f_tree_time );
            f_cluster_tree->Branch( "duration", &f_tree_duration );
            f_cluster_tree->Branch( "frequency", &f_tree_frequency );
        }

        if( f_line_file.size() > 0 )
        {
            f_line_stream = new TFile( f_line_file.c_str(), "RECREATE" );
            f_line_label = new TObjString( "midge_lines" );
            f_line_tree = new TTree( "lines", "lines" );
            f_line_tree->SetDirectory( f_line_stream );
            f_line_tree->Branch( "id", &f_tree_id );
            f_line_tree->Branch( "score", &f_tree_score );
            f_line_tree->Branch( "time", &f_tree_time );
            f_line_tree->Branch( "duration", &f_tree_duration );
            f_line_tree->Branch( "frequency", &f_tree_frequency );
            f_line_tree->Branch( "slope", &f_tree_slope );
            f_line_tree->Branch( "correlation", &f_tree_correlation );
            f_line_tree->Branch( "deviation", &f_tree_deviation );
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

        if( f_background_file.size() == 0 )
        {
            throw error() << "cannot start candiate consumer <" << get_name() << "> with no background file set";
        }

        TFile* t_stream = new TFile( f_background_file.c_str(), "READ" );
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
        cluster::set_slope( f_cluster_slope );
        cluster::set_spread( f_cluster_spread );
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

    bool rf_event_consumer::execute_consumer()
    {
        msg_warning( coremsg, "** START **" << eom );

        if( f_count == 0 )
        {
            f_minimum_time = in< 0 >()->get_time();
        }
        else
        {
            f_maximum_time = in< 0 >()->get_time();
        }
        f_count++;

        msg_warning( coremsg, "** updating ratios **" << eom );
        register double t_in;
        register double t_background;
        register double t_ratio;
        for( count_t t_index = f_frequency_minimum_index; t_index <= f_frequency_maximum_index; t_index++ )
        {
            t_in = f_in[ t_index ];
            t_background = f_background[ t_index ];
            t_ratio = ((t_in - t_background) / t_background);
            if( t_ratio < f_cluster_add_ratio )
            {
                f_ratio[ t_index ] = -1.;
            }
            else
            {
                f_ratio[ t_index ] = t_ratio;

                f_tree_data_time = f_maximum_time;
                f_tree_data_frequency = f_interval * t_index;
                f_tree_data_ratio = t_ratio;
                f_ratio_tree->Fill();
            }
        }

        msg_warning( coremsg, "** updating lines **" << eom );
        stack< line_it > t_line_complete_stack;
        stack< line_it > t_line_active_stack;
        line* t_line;
        for( line_it t_it = f_active_lines.begin(); t_it != f_active_lines.end(); t_it++ )
        {
            t_line = *t_it;

            msg_warning( coremsg, "**   updating active line <" << t_line->id() << "> **" << eom );
            t_line->update();

            if( t_line->score() < f_line_stop_score )
            {
                msg_warning( coremsg, "**   will complete active line <" << t_line->id() << "> **" << eom );
                t_line_complete_stack.push( t_it );
            }
        }
        for( line_it t_it = f_candidate_lines.begin(); t_it != f_candidate_lines.end(); t_it++ )
        {
            t_line = *t_it;

            msg_warning( coremsg, "**   updating candidate line <" << t_line->id() << "> **" << eom );
            t_line->update();

            if( t_line->score() > f_line_stop_score )
            {
                msg_warning( coremsg, "**   will activate candidate line <" << t_line->id() << "> **" << eom );
                t_line_active_stack.push( t_it );
            }
        }
        msg_warning( coremsg, "** lines updated **" << eom );

        msg_warning( coremsg, "** updating clusters **" << eom );
        stack< cluster_it > t_cluster_complete_stack;
        stack< cluster_it > t_cluster_discard_stack;
        cluster* t_cluster;
        for( cluster_it t_it = f_candidate_clusters.begin(); t_it != f_candidate_clusters.end(); t_it++ )
        {
            t_cluster = *t_it;

            msg_warning( coremsg, "**   updating candidate cluster <" << t_cluster->id() << "> **" << eom );
            t_cluster->update();

            if( t_cluster->score() > f_line_start_score )
            {
                msg_warning( coremsg, "**   will complete candidate cluster <" << t_cluster->id() << "> **" << eom );
                t_cluster_complete_stack.push( t_it );
            }

            if( t_cluster->score() < 0. )
            {
                msg_warning( coremsg, "**   will discard candidate cluster <" << t_cluster->id() << "> **" << eom );
                t_cluster_discard_stack.push( t_it );
            }
        }
        msg_warning( coremsg, "** clusters updated **" << eom );

        msg_warning( coremsg, "** completing lines **" << eom );
        while( t_line_complete_stack.empty() == false )
        {
            t_line = *(t_line_complete_stack.top());
            f_complete_lines.push_back( t_line );
            msg_warning( coremsg, "**   completing line <" << t_line->id() << "> **" << eom );

            f_active_lines.erase( t_line_complete_stack.top() );
            t_line_complete_stack.pop();

            if( f_line_stream != NULL )
            {
                f_tree_id = t_line->id();
                f_tree_score = t_line->score();
                f_tree_time = t_line->time();
                f_tree_duration = t_line->duration();
                f_tree_frequency = t_line->frequency();
                f_tree_slope = t_line->slope();
                f_tree_correlation = t_line->correlation();
                f_tree_deviation = t_line->deviation();
                f_line_tree->Fill();

                stringstream t_name;
                t_name << "line_" << f_tree_id << "_data";
                TTree* t_line_ratio_tree = new TTree( t_name.str().c_str(), t_name.str().c_str() );
                t_line_ratio_tree->SetDirectory( f_line_stream );
                t_line_ratio_tree->Branch( "time", &f_tree_data_time );
                t_line_ratio_tree->Branch( "frequency", &f_tree_data_frequency );
                t_line_ratio_tree->Branch( "ratio", &f_tree_data_ratio );
                for( count_t t_index = 0; t_index < t_line->ratios().size(); t_index++ )
                {
                    f_tree_data_time = t_line->times().at( t_index );
                    f_tree_data_frequency = t_line->frequencies().at( t_index );
                    f_tree_data_ratio = t_line->ratios().at( t_index );
                    t_line_ratio_tree->Fill();
                }
                f_line_stream->cd();
                t_line_ratio_tree->Write();

                msg_warning( coremsg, "**   completed line <" << t_line->id() << "> **" << eom );
            }
        }

        msg_warning( coremsg, "** activating lines **" << eom );
        while( t_line_active_stack.empty() == false )
        {
            t_line = *(t_line_active_stack.top());
            f_active_lines.push_back( t_line );
            msg_warning( coremsg, "**   activating line <" << t_line->id() << "> **" << eom );

            f_candidate_lines.erase( t_line_active_stack.top());
            t_line_active_stack.pop();
        }

        msg_warning( coremsg, "** completing clusters **" << eom );
        while( t_cluster_complete_stack.empty() == false )
        {
            t_cluster = *(t_cluster_complete_stack.top());
            f_complete_clusters.push_back( t_cluster );
            msg_warning( coremsg, "**   completing cluster <" << t_cluster->id() << "> **" << eom );

            f_candidate_clusters.erase( t_cluster_complete_stack.top() );
            t_cluster_complete_stack.pop();

            f_candidate_lines.push_back( new line( *t_cluster ) );

            if( f_cluster_stream != NULL )
            {
                f_tree_id = t_cluster->id();
                f_tree_score = t_cluster->score();
                f_tree_time = t_cluster->time();
                f_tree_duration = t_cluster->duration();
                f_tree_frequency = t_cluster->frequency();
                f_cluster_tree->Fill();

                stringstream t_name;
                t_name << "cluster_" << f_tree_id << "_data";
                TTree* t_cluster_ratio_tree = new TTree( t_name.str().c_str(), t_name.str().c_str() );
                t_cluster_ratio_tree->SetDirectory( f_cluster_stream );
                t_cluster_ratio_tree->Branch( "time", &f_tree_data_time );
                t_cluster_ratio_tree->Branch( "frequency", &f_tree_data_frequency );
                t_cluster_ratio_tree->Branch( "ratio", &f_tree_data_ratio );
                for( count_t t_index = 0; t_index < t_cluster->ratios().size(); t_index++ )
                {
                    f_tree_data_time = t_cluster->times().at( t_index );
                    f_tree_data_frequency = t_cluster->frequencies().at( t_index );
                    f_tree_data_ratio = t_cluster->ratios().at( t_index );
                    t_cluster_ratio_tree->Fill();
                }
                f_cluster_stream->cd();
                t_cluster_ratio_tree->Write();
            }

            msg_warning( coremsg, "**   completed cluster <" << t_cluster->id() << "> **" << eom );
        }

        msg_warning( coremsg, "** discarding clusters **" << eom );
        while( t_cluster_discard_stack.empty() == false )
        {
            t_cluster = *(t_cluster_discard_stack.top());
            msg_warning( coremsg, "**   discarding cluster <" << t_cluster->id() << "> **" << eom );

            f_candidate_clusters.erase( t_cluster_discard_stack.top() );
            t_cluster_discard_stack.pop();

            delete t_cluster;
        }

        msg_warning( coremsg, "** adding clusters **" << eom );
        for( count_t t_index = f_frequency_minimum_index; t_index <= f_frequency_maximum_index; t_index++ )
        {
            if( f_ratio[ t_index ] > 0. )
            {
                t_cluster = new cluster( f_maximum_time, t_index * f_interval );
                f_candidate_clusters.push_back( t_cluster );
            }
        }

        msg_warning( coremsg, "** FINISH **" << eom );
        return true;
    }

    bool rf_event_consumer::stop_consumer()
    {
        f_ratio_stream->cd();
        f_ratio_label->Write();
        f_ratio_tree->Write();
        f_ratio_stream->Close();
        delete f_ratio_stream;
        f_ratio_label = NULL;
        f_ratio_tree = NULL;
        f_ratio_stream = NULL;

        f_cluster_stream->cd();
        f_cluster_label->Write();
        f_cluster_tree->Write();
        f_cluster_stream->Close();
        delete f_cluster_stream;
        f_cluster_label = NULL;
        f_cluster_tree = NULL;
        f_cluster_stream = NULL;

        f_line_stream->cd();
        f_line_label->Write();
        f_line_tree->Write();
        f_line_stream->Close();
        delete f_line_stream;
        f_line_label = NULL;
        f_line_tree = NULL;
        f_line_stream = NULL;

        line_it t_line_it;
        line* t_line;
        while( f_complete_lines.empty() == false )
        {
            t_line_it = f_complete_lines.begin();
            t_line = (*t_line_it);
            f_complete_lines.pop_front();
            delete t_line;
        }
        while( f_active_lines.empty() == false )
        {
            t_line_it = f_active_lines.begin();
            t_line = (*t_line_it);
            f_active_lines.pop_front();
            delete t_line;
        }
        while( f_candidate_lines.empty() == false )
        {
            t_line_it = f_candidate_lines.begin();
            t_line = (*t_line_it);
            f_candidate_lines.pop_front();
            delete t_line;
        }

        cluster_it t_cluster_it;
        cluster* t_cluster;
        while( f_complete_clusters.empty() == false )
        {
            t_cluster_it = f_complete_clusters.begin();
            t_cluster = (*t_cluster_it);
            f_complete_clusters.pop_front();
            delete t_cluster;
        }
        while( f_candidate_clusters.empty() == false )
        {
            t_cluster_it = f_candidate_clusters.begin();
            t_cluster = (*t_cluster_it);
            f_candidate_clusters.pop_front();
            delete t_cluster;
        }


        if( f_plot == true )
        {
            if( f_ratio_file.size() > 0 )
            {
                TFile* t_threshold_file = new TFile( f_ratio_file.c_str(), "READ" );
                TTree* t_threshold_tree = (TTree*) (t_threshold_file->Get( "ratios" ));
                Long64_t t_threshold_entries = t_threshold_tree->GetEntries();
                t_threshold_tree->SetBranchAddress( "time", &f_tree_data_time );
                t_threshold_tree->SetBranchAddress( "frequency", &f_tree_data_frequency );
                t_threshold_tree->SetBranchAddress( "ratio", &f_tree_data_ratio );

                plot::abscissa t_threshold_times( t_threshold_entries );
                t_threshold_times.title() = string( "Time [sec]" );
                t_threshold_times.count() = f_count;
                t_threshold_times.low() = f_minimum_time;
                t_threshold_times.high() = f_maximum_time;

                plot::abscissa t_threshold_frequencies( t_threshold_entries );
                t_threshold_frequencies.title() = string( "Frequency [Hz]" );
                t_threshold_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
                t_threshold_frequencies.low() = f_frequency_minimum_index * f_interval;
                t_threshold_frequencies.high() = f_frequency_maximum_index * f_interval;

                plot::ordinate t_threshold_values( t_threshold_entries );
                t_threshold_values.title() = f_axis_ratio_title;

                for( Long64_t t_index = 0; t_index < t_threshold_entries; t_index++ )
                {
                    t_threshold_tree->GetEntry( t_index );

                    t_threshold_times.values().at( t_index ) = f_tree_data_time;
                    t_threshold_frequencies.values().at( t_index ) = f_tree_data_frequency;
                    t_threshold_values.values().at( t_index ) = f_tree_data_ratio;
                }

                t_threshold_file->Close();
                delete t_threshold_file;

                plot::get_instance()->plot_two_dimensional( f_plot_ratio_key, f_plot_ratio_name, f_chart_ratio_title, t_threshold_times, t_threshold_frequencies, t_threshold_values );
            }

            if( f_cluster_file.size() > 0 )
            {
                TFile* t_cluster_file = new TFile( f_cluster_file.c_str(), "READ" );
                TTree* t_cluster_tree = (TTree*) (t_cluster_file->Get( "clusters" ));
                Long64_t t_cluster_entries = t_cluster_tree->GetEntries();
                t_cluster_tree->SetBranchAddress( "id", &f_tree_id );
                t_cluster_tree->SetBranchAddress( "score", &f_tree_score );
                t_cluster_tree->SetBranchAddress( "time", &f_tree_time );
                t_cluster_tree->SetBranchAddress( "duration", &f_tree_duration );
                t_cluster_tree->SetBranchAddress( "frequency", &f_tree_frequency );

                TTree* t_cluster_data_tree;
                Long64_t t_cluster_data_entries;
                stringstream t_cluster_data_name;
                plot::abscissa t_cluster_data_times;
                plot::abscissa t_cluster_data_frequencies;
                plot::ordinate t_cluster_data_values;
                plot::abscissa t_cluster_times;
                plot::abscissa t_cluster_frequencies;
                for( Long64_t t_cluster_index = 0; t_cluster_index < t_cluster_entries; t_cluster_index++ )
                {
                    t_cluster_tree->GetEntry( t_cluster_index );

                    t_cluster_data_name << "cluster_" << f_tree_id << "_data";

                    t_cluster_data_tree = (TTree*) (t_cluster_file->Get( t_cluster_data_name.str().c_str() ) );
                    t_cluster_data_entries = t_cluster_data_tree->GetEntries();
                    t_cluster_data_tree->SetBranchAddress( "time", &f_tree_data_time );
                    t_cluster_data_tree->SetBranchAddress( "frequency", &f_tree_data_frequency );
                    t_cluster_data_tree->SetBranchAddress( "ratio", &f_tree_data_ratio );

                    t_cluster_data_times( t_cluster_data_entries );
                    t_cluster_data_times.title() = string( "Time [sec]" );
                    t_cluster_data_times.count() = f_count;
                    t_cluster_data_times.low() = f_minimum_time;
                    t_cluster_data_times.high() = f_maximum_time;

                    t_cluster_data_frequencies( t_cluster_data_entries );
                    t_cluster_data_frequencies.title() = string( "Frequency [Hz]" );
                    t_cluster_data_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
                    t_cluster_data_frequencies.low() = f_frequency_minimum_index * f_interval;
                    t_cluster_data_frequencies.high() = f_frequency_maximum_index * f_interval;

                    t_cluster_data_values( t_cluster_data_entries );
                    t_cluster_data_values.title() = f_axis_cluster_title;

                    for( Long64_t t_cluster_data_index = 0; t_cluster_data_index < t_cluster_data_entries; t_cluster_data_index++ )
                    {
                        t_cluster_data_tree->GetEntry( t_cluster_data_index );
                        t_cluster_data_times.values().at( t_cluster_data_index ) = f_tree_data_time;
                        t_cluster_data_frequencies.values().at( t_cluster_data_index ) = f_tree_data_frequency;
                        t_cluster_data_values.values().at( t_cluster_data_index ) = f_tree_score;
                    }

                    plot::get_instance()->plot_two_dimensional( f_plot_cluster_key, f_plot_cluster_name, f_chart_cluster_title, t_cluster_data_times, t_cluster_data_frequencies, t_cluster_data_values );

                    t_cluster_times( 2 );
                    t_cluster_times.title() = string( "Time [sec]" );
                    t_cluster_times.count() = f_count;
                    t_cluster_times.low() = f_minimum_time;
                    t_cluster_times.high() = f_maximum_time;
                    t_cluster_times.values().at( 0 ) = f_tree_time;
                    t_cluster_times.values().at( 1 ) = f_tree_time + f_tree_duration;

                    t_cluster_frequencies( 2 );
                    t_cluster_frequencies.title() = string( "Frequency [Hz]" );
                    t_cluster_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
                    t_cluster_frequencies.low() = f_frequency_minimum_index * f_interval;
                    t_cluster_frequencies.high() = f_frequency_maximum_index * f_interval;
                    t_cluster_frequencies.values().at( 0 ) = f_tree_frequency;
                    t_cluster_frequencies.values().at( 1 ) = f_tree_frequency + f_cluster_slope * f_tree_duration;

                    plot::get_instance()->graph_two_dimensional( f_plot_cluster_key, f_plot_cluster_name, f_chart_cluster_title, t_cluster_times, t_cluster_frequencies );

                    t_cluster_data_name.str( "" );
                    t_cluster_data_name.clear();
                }

                t_cluster_file->Close();
                delete t_cluster_file;
            }

            if( f_line_file.size() > 0 )
            {
                TFile* t_line_file = new TFile( f_line_file.c_str(), "READ" );
                TTree* t_line_tree = (TTree*) (t_line_file->Get( "lines" ));
                Long64_t t_line_entries = t_line_tree->GetEntries();
                t_line_tree->SetBranchAddress( "id", &f_tree_id );
                t_line_tree->SetBranchAddress( "score", &f_tree_score );
                t_line_tree->SetBranchAddress( "time", &f_tree_time );
                t_line_tree->SetBranchAddress( "duration", &f_tree_duration );
                t_line_tree->SetBranchAddress( "frequency", &f_tree_frequency );
                t_line_tree->SetBranchAddress( "slope", &f_tree_slope );
                t_line_tree->SetBranchAddress( "correlation", &f_tree_correlation );
                t_line_tree->SetBranchAddress( "deviation", &f_tree_deviation );

                TTree* t_line_data_tree;
                Long64_t t_line_data_entries;
                stringstream t_line_data_name;
                plot::abscissa t_line_data_times;
                plot::abscissa t_line_data_frequencies;
                plot::ordinate t_line_data_values;
                plot::abscissa t_line_times;
                plot::abscissa t_line_frequencies;
                for( Long64_t t_line_index = 0; t_line_index < t_line_entries; t_line_index++ )
                {
                    t_line_tree->GetEntry( t_line_index );
                    t_line_data_name << "line_" << f_tree_id << "_data";
                    t_line_data_tree = (TTree*) (t_line_file->Get( t_line_data_name.str().c_str() ) );
                    t_line_data_entries = t_line_data_tree->GetEntries();
                    t_line_data_tree->SetBranchAddress( "time", &f_tree_data_time );
                    t_line_data_tree->SetBranchAddress( "frequency", &f_tree_data_frequency );
                    t_line_data_tree->SetBranchAddress( "ratio", &f_tree_data_ratio );

                    t_line_data_times( t_line_data_entries );
                    t_line_data_times.title() = string( "Time [sec]" );
                    t_line_data_times.count() = f_count;
                    t_line_data_times.low() = f_minimum_time;
                    t_line_data_times.high() = f_maximum_time;

                    t_line_data_frequencies( t_line_data_entries );
                    t_line_data_frequencies.title() = string( "Frequency [Hz]" );
                    t_line_data_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
                    t_line_data_frequencies.low() = f_frequency_minimum_index * f_interval;
                    t_line_data_frequencies.high() = f_frequency_maximum_index * f_interval;

                    t_line_data_values( t_line_data_entries );
                    t_line_data_values.title() = f_axis_line_title;

                    for( Long64_t t_line_data_index = 0; t_line_data_index < t_line_data_entries; t_line_data_index++ )
                    {
                        t_line_data_tree->GetEntry( t_line_data_index );
                        t_line_data_times.values().at( t_line_data_index ) = f_tree_data_time;
                        t_line_data_frequencies.values().at( t_line_data_index ) = f_tree_data_frequency;
                        t_line_data_values.values().at( t_line_data_index ) = f_tree_score;
                    }

                    plot::get_instance()->plot_two_dimensional( f_plot_line_key, f_plot_line_name, f_chart_line_title, t_line_data_times, t_line_data_frequencies, t_line_data_values );

                    t_line_times( 2 );
                    t_line_times.title() = string( "Time [sec]" );
                    t_line_times.count() = f_count;
                    t_line_times.low() = f_minimum_time;
                    t_line_times.high() = f_maximum_time;
                    t_line_times.values().at( 0 ) = f_tree_time;
                    t_line_times.values().at( 1 ) = f_tree_time + f_tree_duration;

                    t_line_frequencies( 2 );
                    t_line_frequencies.title() = string( "Frequency [Hz]" );
                    t_line_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
                    t_line_frequencies.low() = f_frequency_minimum_index * f_interval;
                    t_line_frequencies.high() = f_frequency_maximum_index * f_interval;
                    t_line_frequencies.values().at( 0 ) = f_tree_frequency;
                    t_line_frequencies.values().at( 1 ) = f_tree_frequency + f_tree_slope * f_tree_duration;

                    plot::get_instance()->graph_two_dimensional( f_plot_line_key, f_plot_line_name, f_chart_line_title, t_line_times, t_line_frequencies );

                    t_line_data_name.str( "" );
                    t_line_data_name.clear();
                }

                t_line_file->Close();
                delete t_line_file;
            }

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
        cluster::set_slope( 0. );
        cluster::set_spread( 1. );
        cluster::set_add_coefficient( 1. );
        cluster::set_add_power( 0. );
        cluster::set_gap_coefficient( 2. );
        cluster::set_gap_power( 0. );
        cluster::set_id( 0 );

        return true;
    }

    //*******
    //cluster
    //*******

    void rf_event_consumer::cluster::set_time( real_t* p_time )
    {
        s_time = p_time;
        return;
    }
    void rf_event_consumer::cluster::set_ratio( real_t* p_ratio )
    {
        s_ratio = p_ratio;
        return;
    }
    void rf_event_consumer::cluster::set_interval( const real_t& p_interval )
    {
        s_interval = p_interval;
        return;
    }
    void rf_event_consumer::cluster::set_min_index( const count_t& p_min_index )
    {
        s_min_index = p_min_index;
        return;
    }
    void rf_event_consumer::cluster::set_max_index( const count_t& p_max_index )
    {
        s_max_index = p_max_index;
        return;
    }
    void rf_event_consumer::cluster::set_slope( const real_t& p_slope )
    {
        s_slope = p_slope;
        return;
    }
    void rf_event_consumer::cluster::set_spread( const real_t& p_spread )
    {
        s_spread = p_spread;
        return;
    }
    void rf_event_consumer::cluster::set_add_coefficient( const real_t& p_add_coefficient )
    {
        s_add_coefficient = p_add_coefficient;
        return;
    }
    void rf_event_consumer::cluster::set_add_power( const real_t& p_add_power )
    {
        s_add_power = p_add_power;
        return;
    }
    void rf_event_consumer::cluster::set_gap_coefficient( const real_t& p_gap_coefficient )
    {
        s_gap_coefficient = p_gap_coefficient;
        return;
    }
    void rf_event_consumer::cluster::set_gap_power( const real_t& p_gap_power )
    {
        s_gap_power = p_gap_power;
        return;
    }
    void rf_event_consumer::cluster::set_id( const count_t& p_id )
    {
        s_id = p_id;
        return;
    }

    real_t* rf_event_consumer::cluster::s_time = NULL;
    real_t* rf_event_consumer::cluster::s_ratio = NULL;
    real_t rf_event_consumer::cluster::s_interval = 1.;
    count_t rf_event_consumer::cluster::s_min_index = 0;
    count_t rf_event_consumer::cluster::s_max_index = 0;
    real_t rf_event_consumer::cluster::s_slope = 0.;
    real_t rf_event_consumer::cluster::s_spread = 1.;
    real_t rf_event_consumer::cluster::s_add_coefficient = 1.;
    real_t rf_event_consumer::cluster::s_add_power = 0.;
    real_t rf_event_consumer::cluster::s_gap_coefficient = 2.;
    real_t rf_event_consumer::cluster::s_gap_power = 0.;
    count_t rf_event_consumer::cluster::s_id = 0;

    rf_event_consumer::cluster::cluster( const real_t& p_time, const real_t& p_frequency ) :
            f_id( s_id++ ),
            f_count( 0. ),
            f_score( 0. ),
            f_time( p_time ),
            f_duration( 0. ),
            f_frequency( p_frequency ),
            f_times(),
            f_frequencies(),
            f_ratios(),
            f_gaps(),
            f_w_sum( 0. ),
            f_wt_sum( 0. ),
            f_wf_sum( 0. ),
            f_add_count_sum( 0. ),
            f_add_score_sum( 0. ),
            f_gap_count_sum( 0. ),
            f_gap_score_sum( 0. ),
            f_gap_count_current( 0. ),
            f_gap_score_current( 0. ),
            f_state( e_in_add )
    {
        msg_warning( coremsg, "cluster <" << f_id << "> created" << eom );
        msg_warning( coremsg, "  time <" << p_time << ">" << eom );
        msg_warning( coremsg, "  frequency <" << p_frequency << ">" << eom );

        update();
    }
    rf_event_consumer::cluster::~cluster()
    {
        msg_warning( coremsg, "cluster <" << f_id << "> destroyed" << eom );
    }

    void rf_event_consumer::cluster::update()
    {
        msg_warning( coremsg, "cluster <" << f_id << "> updating:" << eom );

        count_t t_index;
        real_t t_current_time = *s_time;
        real_t t_current_frequency = f_frequency + s_slope * (t_current_time - f_time);

        msg_warning( coremsg, "  current time is <" << t_current_time << ">" << eom );
        msg_warning( coremsg, "  current frequency is <" << t_current_frequency << ">" << eom );

        // compute the minimum usable index this round
        count_t t_min_index = (count_t) (ceil( (t_current_frequency - 2. * s_spread) / s_interval ));
        if( t_min_index < s_min_index )
        {
            t_min_index = s_min_index;
        }
        msg_warning( coremsg, "  minimum frequency is <" << t_min_index * s_interval << ">" << eom );

        // compute the maximum usable index this round
        count_t t_max_index = (count_t) (floor( (t_current_frequency + 2. * s_spread) / s_interval ));
        if( t_max_index > s_max_index )
        {
            t_max_index = s_max_index;
        }
        msg_warning( coremsg, "  maximum frequency is <" << t_max_index * s_interval << ">" << eom );

        msg_warning( coremsg, "  accumulating points:" << eom );
        register bool_t t_point_found = false;
        register real_t t_point_ratio;
        register real_t t_point_frequency;
        for( t_index = t_min_index; t_index <= t_max_index; t_index++ )
        {
            t_point_ratio = s_ratio[ t_index ];
            t_point_frequency = t_index * s_interval;
            if( t_point_ratio > 0. )
            {
                t_point_found = true;
                s_ratio[ t_index ] = -1.;

                msg_warning( coremsg, "    accumulating point at time <" << t_current_time << "> and frequency <" << t_point_frequency << "> with ratio <" << t_point_ratio << ">" << eom );

                f_w_sum += t_point_ratio;
                f_wt_sum += t_point_ratio * (t_current_time - f_time);
                f_wf_sum += t_point_ratio * t_point_frequency;

                f_ratios.push_back( t_point_ratio );
                f_times.push_back( t_current_time );
                f_frequencies.push_back( t_point_frequency );
            }
        }

        // compute state
        if( t_point_found == true )
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
            msg_warning( coremsg, "  remained in state <add>" << eom );

            msg_warning( coremsg, "  updating properties:" << eom );
            f_duration = (t_current_time - f_time);
            f_frequency = (f_wf_sum - s_slope * f_wt_sum) / f_w_sum;
            t_current_frequency = f_frequency + s_slope * f_duration;
            msg_warning( coremsg, "    duration is now <" << f_duration << ">" << eom );
            msg_warning( coremsg, "    frequency is now <" << f_frequency << ">" << eom );
            msg_warning( coremsg, "    current frequency is now <" << t_current_frequency << ">" << eom );

            msg_warning( coremsg, "  recalculating add score:" << eom );
            f_add_count_sum = 0.;
            f_add_score_sum = 0.;
            register real_t t_score_ratio;
            register real_t t_score_time;
            register real_t t_score_frequency;
            register real_t t_score_center;
            for( t_index = 0; t_index < f_ratios.size(); t_index++ )
            {
                t_score_ratio = f_ratios.at( t_index );
                t_score_time = f_times.at( t_index );
                t_score_frequency = f_frequencies.at( t_index );
                t_score_center = f_frequency + s_slope * (t_score_time - f_time);

                f_add_count_sum += 1.;
                f_add_score_sum += s_add_coefficient * pow( t_score_ratio + 1., s_add_power ) * (.5 + .5 * cos( M_PI * (t_score_frequency - t_score_center) / (2. * s_spread) ));
            }
            msg_warning( coremsg, "    add count sum is now <" << f_add_count_sum << ">" << eom );
            msg_warning( coremsg, "    add score sum is now <" << f_add_score_sum << ">" << eom );
        }
        else if( f_state == e_to_add )
        {
            msg_warning( coremsg, "  switched to state <add>" << eom );

            msg_warning( coremsg, "  updating properties:" << eom );
            f_duration = (t_current_time - f_time);
            f_frequency = (f_wf_sum - s_slope * f_wt_sum) / f_w_sum;
            t_current_frequency = f_frequency + s_slope * f_duration;
            msg_warning( coremsg, "    duration is now <" << f_duration << ">" << eom );
            msg_warning( coremsg, "    frequency is now <" << f_frequency << ">" << eom );
            msg_warning( coremsg, "    current frequency is now <" << t_current_frequency << ">" << eom );

            msg_warning( coremsg, "  recalculating add score:" << eom );
            f_add_count_sum = 0.;
            f_add_score_sum = 0.;
            register real_t t_score_ratio;
            register real_t t_score_time;
            register real_t t_score_frequency;
            register real_t t_score_center;
            for( t_index = 0; t_index < f_ratios.size(); t_index++ )
            {
                t_score_ratio = f_ratios.at( t_index );
                t_score_time = f_times.at( t_index );
                t_score_frequency = f_frequencies.at( t_index );
                t_score_center = f_frequency + s_slope * (t_score_time - f_time);

                f_add_count_sum += 1.;
                f_add_score_sum += s_add_coefficient * pow( t_score_ratio + 1., s_add_power ) * (.5 + .5 * cos( M_PI * (t_score_frequency - t_score_center) / (2. * s_spread) ));
            }
            msg_warning( coremsg, "    add count sum is now <" << f_add_count_sum << ">" << eom );
            msg_warning( coremsg, "    add score sum is now <" << f_add_score_sum << ">" << eom );

            msg_warning( coremsg, "  updating gap score:" << eom );
            f_gap_count_sum += f_gap_count_current;
            f_gap_score_sum += f_gap_score_current;
            f_gap_count_current = 0.;
            f_gap_score_current = 0.;
            msg_warning( coremsg, "    gap count sum is now <" << f_gap_count_sum << ">" << eom );
            msg_warning( coremsg, "    gap score sum is now <" << f_gap_score_sum << ">" << eom );
        }
        else if( f_state == e_in_gap )
        {
            msg_warning( coremsg, "  remained in state <gap>" << eom );

            msg_warning( coremsg, "  updating gap score:" << eom );
            f_gaps.back() += 1.;
            f_gap_count_current += 1.;
            f_gap_score_current = s_gap_coefficient * pow( f_gap_count_current, s_gap_power );
            msg_warning( coremsg, "  gap count is <" << f_gap_count_current << ">" << eom );
            msg_warning( coremsg, "  gap score is <" << f_gap_score_current << ">" << eom );
        }
        else if( f_state == e_to_gap )
        {
            msg_warning( coremsg, "  switched to state <gap>" << eom );

            msg_warning( coremsg, "  updating gap score:" << eom );
            f_gaps.push_back( 1. );
            f_gap_count_current = 1.;
            f_gap_score_current = s_gap_coefficient * pow( f_gap_count_current, s_gap_power );
            msg_warning( coremsg, "  gap count is <" << f_gap_count_current << ">" << eom );
            msg_warning( coremsg, "  gap score is <" << f_gap_score_current << ">" << eom );
        }

        msg_warning( coremsg, "  updating score:" << eom );
        f_count = f_add_count_sum - f_gap_count_sum - f_gap_count_current;
        f_score = f_add_score_sum - f_gap_score_sum - f_gap_score_current;
        msg_warning( coremsg, "    count is <" << f_count << ">" << eom );
        msg_warning( coremsg, "    score is <" << f_score << ">" << eom );

        return;
    }

    const count_t& rf_event_consumer::cluster::id() const
    {
        return f_id;
    }
    const real_t& rf_event_consumer::cluster::count() const
    {
        return f_count;
    }
    const real_t& rf_event_consumer::cluster::score() const
    {
        return f_score;
    }

    const real_t& rf_event_consumer::cluster::time() const
    {
        return f_time;
    }
    const real_t& rf_event_consumer::cluster::duration() const
    {
        return f_duration;
    }
    const real_t& rf_event_consumer::cluster::frequency() const
    {
        return f_frequency;
    }

    const vector< real_t >& rf_event_consumer::cluster::times() const
    {
        return f_times;
    }
    const vector< real_t >& rf_event_consumer::cluster::frequencies() const
    {
        return f_frequencies;
    }
    const vector< real_t >& rf_event_consumer::cluster::ratios() const
    {
        return f_ratios;
    }
    const vector< real_t >& rf_event_consumer::cluster::gaps() const
    {
        return f_gaps;
    }

    //****
    //line
    //****

    void rf_event_consumer::line::set_time( real_t* p_time )
    {
        s_time = p_time;
        return;
    }
    void rf_event_consumer::line::set_ratio( real_t* p_ratio )
    {
        s_ratio = p_ratio;
        return;
    }
    void rf_event_consumer::line::set_interval( const real_t& p_interval )
    {
        s_interval = p_interval;
        return;
    }
    void rf_event_consumer::line::set_min_index( const count_t& p_min_index )
    {
        s_min_index = p_min_index;
        return;
    }
    void rf_event_consumer::line::set_max_index( const count_t& p_max_index )
    {
        s_max_index = p_max_index;
        return;
    }
    void rf_event_consumer::line::set_tolerance( const real_t& p_tolerance )
    {
        s_tolerance = p_tolerance;
        return;
    }
    void rf_event_consumer::line::set_add_coefficient( const real_t& p_add_coefficient )
    {
        s_add_coefficient = p_add_coefficient;
        return;
    }
    void rf_event_consumer::line::set_add_power( const real_t& p_add_power )
    {
        s_add_power = p_add_power;
        return;
    }
    void rf_event_consumer::line::set_gap_coefficient( const real_t& p_gap_coefficient )
    {
        s_gap_coefficient = p_gap_coefficient;
        return;
    }
    void rf_event_consumer::line::set_gap_power( const real_t& p_gap_power )
    {
        s_gap_power = p_gap_power;
        return;
    }
    void rf_event_consumer::line::set_id( const count_t& p_id )
    {
        s_id = p_id;
        return;
    }

    real_t* rf_event_consumer::line::s_time = NULL;
    real_t* rf_event_consumer::line::s_ratio = NULL;
    real_t rf_event_consumer::line::s_interval = 1.;
    count_t rf_event_consumer::line::s_min_index = 0;
    count_t rf_event_consumer::line::s_max_index = 0;
    real_t rf_event_consumer::line::s_tolerance = 1.;
    real_t rf_event_consumer::line::s_add_coefficient = 1.;
    real_t rf_event_consumer::line::s_add_power = 0.;
    real_t rf_event_consumer::line::s_gap_coefficient = 2.;
    real_t rf_event_consumer::line::s_gap_power = 0.;
    count_t rf_event_consumer::line::s_id = 0;

    rf_event_consumer::line::line( const cluster& p_cluster ) :
            f_id( s_id++ ),
            f_count( 0. ),
            f_score( 0. ),
            f_time( p_cluster.time() ),
            f_duration( p_cluster.duration() ),
            f_frequency( p_cluster.frequency() ),
            f_slope( 0. ),
            f_correlation( 0. ),
            f_deviation( 0. ),
            f_times( p_cluster.times() ),
            f_frequencies( p_cluster.frequencies() ),
            f_ratios( p_cluster.ratios() ),
            f_gaps( p_cluster.gaps() ),
            f_w_sum( 0. ),
            f_wt_sum( 0. ),
            f_wf_sum( 0. ),
            f_wtt_sum( 0. ),
            f_wff_sum( 0. ),
            f_wtf_sum( 0. ),
            f_add_count_sum( 0. ),
            f_add_score_sum( 0. ),
            f_gap_count_sum( 0. ),
            f_gap_score_sum( 0. ),
            f_gap_count_current( 0. ),
            f_gap_score_current( 0. ),
            f_state( e_in_add )
    {
        msg_warning( coremsg, "line <" << f_id << "> created" << eom );

        count_t t_index;

        register real_t t_init_ratio;
        register real_t t_init_time;
        register real_t t_init_frequency;
        register real_t t_init_center;
        register real_t t_init_gap;

        msg_warning( coremsg, "  calculating accumulation:" << eom );
        for( t_index = 0; t_index < f_ratios.size(); t_index++ )
        {
            t_init_ratio = f_ratios.at( t_index );
            t_init_time = f_times.at( t_index );
            t_init_frequency = f_frequencies.at( t_index );

            f_w_sum += t_init_ratio;
            f_wt_sum += t_init_ratio * (t_init_time - f_time);
            f_wf_sum += t_init_ratio * t_init_frequency;
            f_wtt_sum += t_init_ratio * (t_init_time - f_time) * (t_init_time - f_time);
            f_wff_sum += t_init_ratio * t_init_frequency * t_init_frequency;
            f_wtf_sum += t_init_ratio * (t_init_time - f_time) * t_init_frequency;
        }

        msg_warning( coremsg, "  calculating properties:" << eom );
        register real_t t_tt_stat = f_w_sum * f_wtt_sum - f_wt_sum * f_wt_sum;
        register real_t t_ff_stat = f_w_sum * f_wff_sum - f_wf_sum * f_wf_sum;
        register real_t t_tf_stat = f_w_sum * f_wtf_sum - f_wt_sum * f_wf_sum;
        register real_t t_ttf_stat = f_wtt_sum * f_wf_sum - f_wt_sum * f_wtf_sum;
        f_frequency = t_ttf_stat / t_tt_stat;
        f_slope = t_tf_stat / t_tt_stat;
        f_correlation = (t_tf_stat * t_tf_stat) / (t_tt_stat * t_ff_stat);

        register real_t t_dev_stat = f_wff_sum;
        t_dev_stat += f_slope * f_slope * f_wtt_sum;
        t_dev_stat += f_frequency * f_frequency * f_w_sum;
        t_dev_stat -= 2. * f_slope * f_wtf_sum;
        t_dev_stat -= 2. * f_frequency * f_wf_sum;
        t_dev_stat += 2. * f_slope * f_frequency * f_wt_sum;
        f_deviation = sqrt( t_dev_stat / f_w_sum );
        msg_warning( coremsg, "    time is <" << f_time << ">" << eom );
        msg_warning( coremsg, "    duration is <" << f_duration << ">" << eom );
        msg_warning( coremsg, "    frequency is <" << f_frequency << ">" << eom );
        msg_warning( coremsg, "    slope is <" << f_slope << ">" << eom );
        msg_warning( coremsg, "    correlation is <" << f_correlation << ">" << eom );
        msg_warning( coremsg, "    deviation is <" << f_deviation << ">" << eom );

        msg_warning( coremsg, "  calculating add score:" << eom );
        for( t_index = 0; t_index < f_ratios.size(); t_index++ )
        {
            t_init_ratio = f_ratios.at( t_index );
            t_init_time = f_times.at( t_index );
            t_init_frequency = f_frequencies.at( t_index );
            t_init_center = f_frequency + f_slope * (t_init_time - f_time);

            f_add_count_sum += 1.;
            f_add_score_sum += s_add_coefficient * pow( t_init_ratio, s_add_power ) * (.5 + .5 * cos( M_PI * (t_init_frequency - t_init_center) / (2. * s_tolerance * f_deviation) ));
        }
        msg_warning( coremsg, "    add count sum is <" << f_add_count_sum << ">" << eom );
        msg_warning( coremsg, "    add score sum is <" << f_add_score_sum << ">" << eom );

        msg_warning( coremsg, "  calculating gap score:" << eom );
        for( t_index = 0; t_index < f_gaps.size(); t_index++ )
        {
            t_init_gap = f_gaps.at( t_index );

            f_gap_count_sum += t_init_gap;
            f_gap_score_sum += s_gap_coefficient * pow( t_init_gap, s_gap_power );
        }
        msg_warning( coremsg, "    gap count sum is <" << f_gap_count_sum << ">" << eom );
        msg_warning( coremsg, "    gap score sum is <" << f_gap_score_sum << ">" << eom );

        msg_warning( coremsg, "  calculating score:" << eom );
        f_count = f_add_count_sum - f_gap_count_sum;
        f_score = f_add_score_sum - f_gap_score_sum;
        msg_warning( coremsg, "    count is <" << f_count << ">" << eom );
        msg_warning( coremsg, "    score is <" << f_score << ">" << eom );

    }
    rf_event_consumer::line::~line()
    {
        msg_warning( coremsg, "line <" << f_id << "> destroyed" << eom );
    }

    void rf_event_consumer::line::update()
    {
        msg_warning( coremsg, "line <" << f_id << "> updating:" << eom );

        count_t t_index;
        real_t t_current_time = *s_time;
        real_t t_current_frequency = f_frequency + f_slope * (t_current_time - f_time);

        msg_warning( coremsg, "  current time is <" << t_current_time << ">" << eom );
        msg_warning( coremsg, "  current frequency is <" << t_current_frequency << ">" << eom );

        // compute the minimum usable index this round
        count_t t_min_index = (count_t) (ceil( (t_current_frequency - 2. * s_tolerance * f_deviation) / s_interval ));
        if( t_min_index < s_min_index )
        {
            t_min_index = s_min_index;
        }
        msg_warning( coremsg, "  minimum frequency is <" << t_min_index * s_interval << ">" << eom );

        // compute the maximum usable index this round
        count_t t_max_index = (count_t) (floor( (t_current_frequency + 2. * s_tolerance * f_deviation) / s_interval ));
        if( t_max_index > s_max_index )
        {
            t_max_index = s_max_index;
        }
        msg_warning( coremsg, "  maximum frequency is <" << t_max_index * s_interval << ">" << eom );

        msg_warning( coremsg, "  accumulating points:" << eom );
        register bool_t t_point_found = false;
        register real_t t_point_ratio;
        register real_t t_point_frequency;
        for( t_index = t_min_index; t_index <= t_max_index; t_index++ )
        {
            t_point_ratio = s_ratio[ t_index ];
            t_point_frequency = t_index * s_interval;
            if( t_point_ratio > 0. )
            {
                t_point_found = true;
                s_ratio[ t_index ] = -1.;

                msg_warning( coremsg, "    accumulating point at time <" << t_current_time << "> and frequency <" << t_point_frequency << "> with ratio <" << t_point_ratio << ">" << eom );

                f_w_sum += t_point_ratio;
                f_wt_sum += t_point_ratio * (t_current_time - f_time);
                f_wf_sum += t_point_ratio * t_point_frequency;
                f_wtt_sum += t_point_ratio * (t_current_time - f_time) * (t_current_time - f_time);
                f_wff_sum += t_point_ratio * t_point_frequency * t_point_frequency;
                f_wtf_sum += t_point_ratio * (t_current_time - f_time) * t_point_frequency;

                f_ratios.push_back( t_point_ratio );
                f_times.push_back( t_current_time );
                f_frequencies.push_back( t_point_frequency );
            }
        }

        // compute state
        if( t_point_found == true )
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
            msg_warning( coremsg, "  remained in state <add>" << eom );

            msg_warning( coremsg, "  updating properties:" << eom );
            register real_t t_tt_stat = f_w_sum * f_wtt_sum - f_wt_sum * f_wt_sum;
            register real_t t_ff_stat = f_w_sum * f_wff_sum - f_wf_sum * f_wf_sum;
            register real_t t_tf_stat = f_w_sum * f_wtf_sum - f_wt_sum * f_wf_sum;
            register real_t t_ttf_stat = f_wtt_sum * f_wf_sum - f_wt_sum * f_wtf_sum;
            f_frequency = t_ttf_stat / t_tt_stat;
            f_slope = t_tf_stat / t_tt_stat;
            f_correlation = (t_tf_stat * t_tf_stat) / (t_tt_stat * t_ff_stat);

            register real_t t_dev_stat = f_wff_sum;
            t_dev_stat += f_slope * f_slope * f_wtt_sum;
            t_dev_stat += f_frequency * f_frequency * f_w_sum;
            t_dev_stat -= 2. * f_slope * f_wtf_sum;
            t_dev_stat -= 2. * f_frequency * f_wf_sum;
            t_dev_stat += 2. * f_slope * f_frequency * f_wt_sum;
            f_deviation = sqrt( t_dev_stat / f_w_sum );

            f_duration = (t_current_time - f_time);
            t_current_frequency = f_frequency + f_slope * f_duration;
            msg_warning( coremsg, "    duration is now <" << f_duration << ">" << eom );
            msg_warning( coremsg, "    frequency is now <" << f_frequency << ">" << eom );
            msg_warning( coremsg, "    current frequency is now <" << t_current_frequency << ">" << eom );
            msg_warning( coremsg, "    slope is now <" << f_slope << ">" << eom );
            msg_warning( coremsg, "    correlation is now <" << f_correlation << ">" << eom );
            msg_warning( coremsg, "    deviation is now <" << f_deviation << ">" << eom );

            msg_warning( coremsg, "  recalculating add score:" << eom );
            f_add_count_sum = 0.;
            f_add_score_sum = 0.;
            register real_t t_score_ratio;
            register real_t t_score_time;
            register real_t t_score_frequency;
            register real_t t_score_center;
            for( t_index = 0; t_index < f_ratios.size(); t_index++ )
            {
                t_score_ratio = f_ratios.at( t_index );
                t_score_time = f_times.at( t_index );
                t_score_frequency = f_frequencies.at( t_index );
                t_score_center = f_frequency + f_slope * (t_score_time - f_time);

                f_add_count_sum += 1.;
                f_add_score_sum += s_add_coefficient * pow( t_score_ratio + 1., s_add_power ) * (.5 + .5 * cos( M_PI * (t_score_frequency - t_score_center) / (2. * s_tolerance * f_deviation) ));
            }
            msg_warning( coremsg, "    add count sum is now <" << f_add_count_sum << ">" << eom );
            msg_warning( coremsg, "    add score sum is now <" << f_add_score_sum << ">" << eom );
        }
        else if( f_state == e_to_add )
        {
            msg_warning( coremsg, "  switched to state <add>" << eom );

            msg_warning( coremsg, "  updating properties:" << eom );
            register real_t t_tt_stat = f_w_sum * f_wtt_sum - f_wt_sum * f_wt_sum;
            register real_t t_ff_stat = f_w_sum * f_wff_sum - f_wf_sum * f_wf_sum;
            register real_t t_tf_stat = f_w_sum * f_wtf_sum - f_wt_sum * f_wf_sum;
            register real_t t_ttf_stat = f_wtt_sum * f_wf_sum - f_wt_sum * f_wtf_sum;
            f_frequency = t_ttf_stat / t_tt_stat;
            f_slope = t_tf_stat / t_tt_stat;
            f_correlation = (t_tf_stat * t_tf_stat) / (t_tt_stat * t_ff_stat);

            register real_t t_dev_stat = f_wff_sum;
            t_dev_stat += f_slope * f_slope * f_wtt_sum;
            t_dev_stat += f_frequency * f_frequency * f_w_sum;
            t_dev_stat -= 2. * f_slope * f_wtf_sum;
            t_dev_stat -= 2. * f_frequency * f_wf_sum;
            t_dev_stat += 2. * f_slope * f_frequency * f_wt_sum;
            f_deviation = sqrt( t_dev_stat / f_w_sum );

            f_duration = (t_current_time - f_time);
            t_current_frequency = f_frequency + f_slope * f_duration;
            msg_warning( coremsg, "    duration is now <" << f_duration << ">" << eom );
            msg_warning( coremsg, "    frequency is now <" << f_frequency << ">" << eom );
            msg_warning( coremsg, "    current frequency is now <" << t_current_frequency << ">" << eom );
            msg_warning( coremsg, "    slope is now <" << f_slope << ">" << eom );
            msg_warning( coremsg, "    correlation is now <" << f_correlation << ">" << eom );
            msg_warning( coremsg, "    deviation is now <" << f_deviation << ">" << eom );

            msg_warning( coremsg, "  recalculating add score:" << eom );
            f_add_count_sum = 0.;
            f_add_score_sum = 0.;
            register real_t t_score_ratio;
            register real_t t_score_time;
            register real_t t_score_frequency;
            register real_t t_score_center;
            for( t_index = 0; t_index < f_ratios.size(); t_index++ )
            {
                t_score_ratio = f_ratios.at( t_index );
                t_score_time = f_times.at( t_index );
                t_score_frequency = f_frequencies.at( t_index );
                t_score_center = f_frequency + f_slope * (t_score_time - f_time);

                f_add_count_sum += 1.;
                f_add_score_sum += s_add_coefficient * pow( t_score_ratio + 1., s_add_power ) * (.5 + .5 * cos( M_PI * (t_score_frequency - t_score_center) / (2. * s_tolerance * f_deviation) ));
            }
            msg_warning( coremsg, "    add count sum is now <" << f_add_count_sum << ">" << eom );
            msg_warning( coremsg, "    add score sum is now <" << f_add_score_sum << ">" << eom );

            msg_warning( coremsg, "  updating gap score:" << eom );
            f_gap_count_sum += f_gap_count_current;
            f_gap_score_sum += f_gap_score_current;
            f_gap_count_current = 0.;
            f_gap_score_current = 0.;
            msg_warning( coremsg, "    gap count sum is now <" << f_gap_count_sum << ">" << eom );
            msg_warning( coremsg, "    gap score sum is now <" << f_gap_score_sum << ">" << eom );
        }
        else if( f_state == e_in_gap )
        {
            msg_warning( coremsg, "  remained in state <gap>" << eom );

            msg_warning( coremsg, "  recalculating gap score:" << eom );
            f_gaps.back() += 1.;
            f_gap_count_current += 1.;
            f_gap_score_current = s_gap_coefficient * pow( f_gap_count_current, s_gap_power );
            msg_warning( coremsg, "  gap count is <" << f_gap_count_current << ">" << eom );
            msg_warning( coremsg, "  gap score is <" << f_gap_score_current << ">" << eom );
        }
        else if( f_state == e_to_gap )
        {
            msg_warning( coremsg, "  switched to state <gap>" << eom );

            msg_warning( coremsg, "  recalculating gap score:" << eom );
            f_gaps.push_back( 1. );
            f_gap_count_current = 1.;
            f_gap_score_current = s_gap_coefficient * pow( f_gap_count_current, s_gap_power );
            msg_warning( coremsg, "  gap count is <" << f_gap_count_current << ">" << eom );
            msg_warning( coremsg, "  gap score is <" << f_gap_score_current << ">" << eom );
        }

        msg_warning( coremsg, "  updating score:" << eom );
        f_count = f_add_count_sum - f_gap_count_sum - f_gap_count_current;
        f_score = f_add_score_sum - f_gap_score_sum - f_gap_score_current;
        msg_warning( coremsg, "    count is <" << f_count << ">" << eom );
        msg_warning( coremsg, "    score is <" << f_score << ">" << eom );

        return;
    }

    const count_t& rf_event_consumer::line::id() const
    {
        return f_id;
    }
    const real_t& rf_event_consumer::line::count() const
    {
        return f_count;
    }
    const real_t& rf_event_consumer::line::score() const
    {
        return f_score;
    }

    const real_t& rf_event_consumer::line::time() const
    {
        return f_time;
    }
    const real_t& rf_event_consumer::line::duration() const
    {
        return f_duration;
    }
    const real_t& rf_event_consumer::line::frequency() const
    {
        return f_frequency;
    }
    const real_t& rf_event_consumer::line::slope() const
    {
        return f_slope;
    }
    const real_t& rf_event_consumer::line::correlation() const
    {
        return f_correlation;
    }
    const real_t& rf_event_consumer::line::deviation() const
    {
        return f_deviation;
    }

    const vector< real_t >& rf_event_consumer::line::times() const
    {
        return f_times;
    }
    const vector< real_t >& rf_event_consumer::line::frequencies() const
    {
        return f_frequencies;
    }
    const vector< real_t >& rf_event_consumer::line::ratios() const
    {
        return f_ratios;
    }
    const vector< real_t >& rf_event_consumer::line::gaps() const
    {
        return f_gaps;
    }

}

