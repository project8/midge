#include "rf_candidate_consumer.hh"

#include "plot.hh"

#include <limits>
using std::numeric_limits;

#include <cmath>

namespace midge
{

    rf_candidate_consumer::rf_candidate_consumer() :
            f_threshold_file( "" ),
            f_background_file( "" ),
            f_frequency_minimum( numeric_limits< real_t >::min() ),
            f_frequency_maximum( numeric_limits< real_t >::max() ),
            f_threshold( -1. ),
            f_cluster_slope( 0. ),
            f_cluster_spread( 1. ),
            f_cluster_add_coefficient( 1. ),
            f_cluster_add_power( 0. ),
            f_cluster_gap_coefficient( 2. ),
            f_cluster_gap_power( 0. ),
            f_cluster_score_up( 10. ),
            f_cluster_score_down( 0. ),
            f_plot( false ),
            f_plot_threshold_key( "" ),
            f_plot_threshold_name( "" ),
            f_chart_threshold_title( "" ),
            f_axis_threshold_title( "" ),
            f_plot_cluster_key( "" ),
            f_plot_cluster_name( "" ),
            f_chart_cluster_title( "" ),
            f_axis_cluster_title( "" ),
            f_threshold_stream( NULL ),
            f_threshold_label( NULL ),
            f_threshold_tree( NULL ),
            f_cluster_stream( NULL ),
            f_cluster_label( NULL ),
            f_cluster_tree( NULL ),
            f_tree_time( 0. ),
            f_tree_frequency( 0. ),
            f_tree_value( 0. ),
            f_tree_id( 0 ),
            f_tree_score( 0. ),
            f_size( 0 ),
            f_interval( 1. ),
            f_in( NULL ),
            f_signal( NULL ),
            f_background( NULL ),
            f_frequency_minimum_index( 0 ),
            f_frequency_maximum_index( numeric_limits< count_t >::max() ),
            f_minimum_time( 0. ),
            f_current_time( 0. ),
            f_count( 0 ),
            f_active_clusters( 0 ),
            f_completed_clusters( 0 )
    {
    }
    rf_candidate_consumer::~rf_candidate_consumer()
    {
    }

    bool rf_candidate_consumer::start_consumer()
    {
        if( f_plot == true )
        {
            plot::get_instance()->initialize();
        }

        f_threshold_stream = new TFile( f_threshold_file.c_str(), "RECREATE" );

        f_threshold_label = new TObjString( "midge_threshold" );

        f_threshold_tree = new TTree( get_name().c_str(), get_name().c_str() );
        f_threshold_tree->SetDirectory( f_threshold_stream );
        f_threshold_tree->Branch( "time", &f_tree_time );
        f_threshold_tree->Branch( "frequency", &f_tree_frequency );
        f_threshold_tree->Branch( "value", &f_tree_value );

        f_cluster_stream = new TFile( f_cluster_file.c_str(), "RECREATE" );

        f_cluster_label = new TObjString( "midge_cluster" );

        f_cluster_tree = new TTree( get_name().c_str(), get_name().c_str() );
        f_cluster_tree->SetDirectory( f_cluster_stream );
        f_cluster_tree->Branch( "time", &f_tree_time );
        f_cluster_tree->Branch( "frequency", &f_tree_frequency );
        f_cluster_tree->Branch( "value", &f_tree_value );
        f_cluster_tree->Branch( "id", &f_tree_id );
        f_cluster_tree->Branch( "score", &f_tree_score );

        f_size = in< 0 >()->get_size();
        f_interval = in< 0 >()->get_interval();
        f_in = in< 0 >()->raw();

        f_signal = new real_t[ f_size ];
        for( count_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_signal[ t_index ] = -1.;
        }

        f_background = new real_t[ f_size ];
        for( count_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_background[ t_index ] = 0.;
        }

        if( f_background_file.size() == 0 )
        {
            throw error() << "cannot start rf rf threshold consumer <" << get_name() << "> with no background file set";
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
        f_current_time = 0.;
        f_count = 0;

        cluster::set_time( &f_current_time );
        cluster::set_signal( f_signal );
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

        return true;
    }

    bool rf_candidate_consumer::execute_consumer()
    {
        if( f_count == 0 )
        {
            f_minimum_time = in< 0 >()->get_time();
        }
        else
        {
            f_current_time = in< 0 >()->get_time();
        }
        f_count++;

        // compute signal
        register double t_in;
        register double t_background;
        register double t_value;
        for( count_t t_index = f_frequency_minimum_index; t_index <= f_frequency_maximum_index; t_index++ )
        {
            t_in = f_in[ t_index ];
            t_background = f_background[ t_index ];
            t_value = ((t_in - t_background) / t_background) - f_threshold;
            if( t_value < 0. )
            {
                f_signal[ t_index ] = -1.;
            }
            else
            {
                f_signal[ t_index ] = t_value;

                f_tree_time = in< 0 >()->get_time();
                f_tree_frequency = f_interval * t_index;
                f_tree_value = t_value;
                f_threshold_tree->Fill();
            }
        }

        // varbs
        cluster_it t_it;
        cluster_it t_rit;
        cluster* t_cluster;

        // update completed clusters
        t_it = f_completed_clusters.begin();
        while( t_it != f_completed_clusters.end() )
        {
            t_cluster = (*t_it);
            t_cluster->update();
            t_it++;
        }

        // update active clusters
        t_it = f_active_clusters.begin();
        while( t_it != f_active_clusters.end() )
        {
            t_cluster = (*t_it);
            t_cluster->update();

            if( t_cluster->score() > f_cluster_score_up )
            {
                t_rit = t_it;
                t_it++;
                f_active_clusters.erase( t_rit );
                f_completed_clusters.push_back( t_cluster );
                continue;
            }

            if( t_cluster->score() < f_cluster_score_down )
            {
                t_rit = t_it;
                t_it++;
                f_active_clusters.erase( t_rit );
                delete t_cluster;
                continue;
            }

            t_it++;
        }

        // create new clusters
        for( count_t t_index = f_frequency_minimum_index; t_index <= f_frequency_maximum_index; t_index++ )
        {
            if( f_signal[ t_index ] > 0. )
            {
                t_cluster = new cluster( t_index );
                f_active_clusters.push_back( t_cluster );
            }
        }

        return true;
    }

    bool rf_candidate_consumer::stop_consumer()
    {
        f_threshold_stream->cd();
        f_threshold_label->Write();
        f_threshold_tree->Write();
        f_threshold_stream->Close();
        delete f_threshold_stream;

        cluster_it t_it;
        cluster* t_cluster;
        count_t t_index;
        while( f_completed_clusters.empty() == false )
        {
            t_it = f_completed_clusters.begin();
            t_cluster = (*t_it);


            f_tree_id = t_cluster->id();
            f_tree_score = t_cluster->score();
            for( t_index = 0; t_index < t_cluster->values().size(); t_index++ )
            {
                f_tree_time = t_cluster->times()[ t_index ];
                f_tree_frequency = t_cluster->frequencies()[ t_index ];
                f_tree_value = t_cluster->values()[ t_index ];
                f_cluster_tree->Fill();
            }

            delete t_cluster;
            f_completed_clusters.pop_front();
        }

        f_cluster_stream->cd();
        f_cluster_label->Write();
        f_cluster_tree->Write();
        f_cluster_stream->Close();
        delete f_cluster_stream;

        if( f_plot == true )
        {
            real_t t_time;
            real_t t_frequency;
            real_t t_value;
            count_t t_id;
            real_t t_score;

            TFile* t_threshold_file = new TFile( f_threshold_file.c_str(), "READ" );

            TTree* t_threshold_tree = (TTree*) (t_threshold_file->Get( get_name().c_str() ));
            Long64_t t_threshold_entries = t_threshold_tree->GetEntries();
            t_threshold_tree->SetBranchAddress( "time", &t_time );
            t_threshold_tree->SetBranchAddress( "frequency", &t_frequency );
            t_threshold_tree->SetBranchAddress( "value", &t_value );

            plot::abscissa t_threshold_times( t_threshold_entries );
            t_threshold_times.title() = string( "Time [sec]" );
            t_threshold_times.count() = f_count;
            t_threshold_times.low() = f_minimum_time;
            t_threshold_times.high() = f_current_time;

            plot::abscissa t_threshold_frequencies( t_threshold_entries );
            t_threshold_frequencies.title() = string( "Frequency [Hz]" );
            t_threshold_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
            t_threshold_frequencies.low() = f_frequency_minimum_index * f_interval;
            t_threshold_frequencies.high() = f_frequency_maximum_index * f_interval;

            plot::ordinate t_threshold_values( t_threshold_entries );
            t_threshold_values.title() = f_axis_threshold_title;

            for( Long64_t t_index = 0; t_index < t_threshold_entries; t_index++ )
            {
                t_threshold_tree->GetEntry( t_index );

                t_threshold_times.values().at( t_index ) = t_time;
                t_threshold_frequencies.values().at( t_index ) = t_frequency;
                t_threshold_values.values().at( t_index ) = t_value;
            }

            t_threshold_file->Close();
            delete t_threshold_file;

            plot::get_instance()->plot_two_dimensional( f_plot_threshold_key, f_plot_threshold_name, f_chart_threshold_title, t_threshold_times, t_threshold_frequencies, t_threshold_values );

            TFile* t_cluster_file = new TFile( f_cluster_file.c_str(), "READ" );

            TTree* t_cluster_tree = (TTree*) (t_cluster_file->Get( get_name().c_str() ));
            Long64_t t_cluster_entries = t_cluster_tree->GetEntries();
            t_cluster_tree->SetBranchAddress( "time", &t_time );
            t_cluster_tree->SetBranchAddress( "frequency", &t_frequency );
            t_cluster_tree->SetBranchAddress( "value", &t_value );
            t_cluster_tree->SetBranchAddress( "id", &t_id );
            t_cluster_tree->SetBranchAddress( "score", &t_score );

            plot::abscissa t_cluster_times( t_cluster_entries );
            t_cluster_times.title() = string( "Time [sec]" );
            t_cluster_times.count() = f_count;
            t_cluster_times.low() = f_minimum_time;
            t_cluster_times.high() = f_current_time;

            plot::abscissa t_cluster_frequencies( t_cluster_entries );
            t_cluster_frequencies.title() = string( "Frequency [Hz]" );
            t_cluster_frequencies.count() = f_frequency_maximum_index - f_frequency_minimum_index + 1;
            t_cluster_frequencies.low() = f_frequency_minimum_index * f_interval;
            t_cluster_frequencies.high() = f_frequency_maximum_index * f_interval;

            plot::ordinate t_cluster_values( t_cluster_entries );
            t_cluster_values.title() = f_axis_cluster_title;

            for( Long64_t t_index = 0; t_index < t_cluster_entries; t_index++ )
            {
                t_cluster_tree->GetEntry( t_index );

                t_cluster_times.values().at( t_index ) = t_time;
                t_cluster_frequencies.values().at( t_index ) = t_frequency;
                t_cluster_values.values().at( t_index ) = t_id;
            }

            t_cluster_file->Close();
            delete t_cluster_file;

            plot::get_instance()->plot_two_dimensional( f_plot_cluster_key, f_plot_cluster_name, f_chart_cluster_title, t_cluster_times, t_cluster_frequencies, t_cluster_values );

            plot::get_instance()->finalize();
        }

        f_size = 0;
        f_interval = 1.;
        f_in = NULL;

        delete[] f_signal;
        f_signal = NULL;

        delete[] f_background;
        f_background = NULL;

        f_frequency_minimum_index = 0;
        f_frequency_maximum_index = numeric_limits< count_t >::max();
        f_minimum_time = 0.;
        f_current_time = 0.;
        f_count = 0;

        cluster::set_time( NULL );
        cluster::set_signal( NULL );
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

    void rf_candidate_consumer::cluster::set_time( real_t* p_time )
    {
        s_time = p_time;
        return;
    }
    void rf_candidate_consumer::cluster::set_signal( real_t* p_signal )
    {
        s_signal = p_signal;
        return;
    }
    void rf_candidate_consumer::cluster::set_interval( const real_t& p_interval )
    {
        s_interval = p_interval;
        return;
    }
    void rf_candidate_consumer::cluster::set_min_index( const count_t& p_min_index )
    {
        s_min_index = p_min_index;
        return;
    }
    void rf_candidate_consumer::cluster::set_max_index( const count_t& p_max_index )
    {
        s_max_index = p_max_index;
        return;
    }
    void rf_candidate_consumer::cluster::set_slope( const real_t& p_slope )
    {
        s_slope = p_slope;
        return;
    }
    void rf_candidate_consumer::cluster::set_spread( const real_t& p_spread )
    {
        s_spread = p_spread;
        return;
    }
    void rf_candidate_consumer::cluster::set_add_coefficient( const real_t& p_add_coefficient )
    {
        s_add_coefficient = p_add_coefficient;
        return;
    }
    void rf_candidate_consumer::cluster::set_add_power( const real_t& p_add_power )
    {
        s_add_power = p_add_power;
        return;
    }
    void rf_candidate_consumer::cluster::set_gap_coefficient( const real_t& p_gap_coefficient )
    {
        s_gap_coefficient = p_gap_coefficient;
        return;
    }
    void rf_candidate_consumer::cluster::set_gap_power( const real_t& p_gap_power )
    {
        s_gap_power = p_gap_power;
        return;
    }
    void rf_candidate_consumer::cluster::set_id( const count_t& p_id )
    {
        s_id = p_id;
        return;
    }

    real_t* rf_candidate_consumer::cluster::s_time = NULL;
    real_t* rf_candidate_consumer::cluster::s_signal = NULL;
    real_t rf_candidate_consumer::cluster::s_interval = 1.;
    count_t rf_candidate_consumer::cluster::s_min_index = 0;
    count_t rf_candidate_consumer::cluster::s_max_index = 0;
    real_t rf_candidate_consumer::cluster::s_slope = 0.;
    real_t rf_candidate_consumer::cluster::s_spread = 1.;
    real_t rf_candidate_consumer::cluster::s_add_coefficient = 1.;
    real_t rf_candidate_consumer::cluster::s_add_power = 0.;
    real_t rf_candidate_consumer::cluster::s_gap_coefficient = 2.;
    real_t rf_candidate_consumer::cluster::s_gap_power = 0.;
    count_t rf_candidate_consumer::cluster::s_id = 0;

    rf_candidate_consumer::cluster::cluster( const count_t& p_index ) :
            f_time( *s_time ),
            f_frequency( p_index * s_interval ),
            f_add_score_sum( 0. ),
            f_gap_score_sum( 0. ),
            f_gap_score( 0. ),
            f_gap_count( 0. ),
            f_id( s_id++ ),
            f_score( 0. ),
            f_times(),
            f_frequencies(),
            f_values()
    {
        update();
    }
    rf_candidate_consumer::cluster::~cluster()
    {
    }

    void rf_candidate_consumer::cluster::update()
    {
        bool t_found = false;
        count_t t_index;

        // compute the minimum usable index this round
        count_t t_min_index = (count_t) (ceil( (f_frequency - 2. * s_spread) / s_interval ));
        if( t_min_index < s_min_index )
        {
            t_min_index = s_min_index;
        }

        // compute the maximum usable index this round
        count_t t_max_index = (count_t) (floor( (f_frequency + 2. * s_spread) / s_interval ));
        if( t_max_index > s_max_index )
        {
            t_max_index = s_max_index;
        }

        // accumulate points
        register real_t t_time;
        register real_t t_frequency;
        register real_t t_value;
        register real_t t_numerator = 0.;
        register real_t t_denominator = 0.;
        for( t_index = t_min_index; t_index <= t_max_index; t_index++ )
        {
            t_value = s_signal[ t_index ];
            if( t_value > 0. )
            {
                t_found = true;
                s_signal[ t_index ] = -1.;

                t_time = *s_time;
                t_frequency = t_index * s_interval;

                t_numerator += t_value * (t_frequency - s_slope * (t_time - f_time));
                t_denominator += t_value;

                f_times.push_back( t_time );
                f_frequencies.push_back( t_frequency );
                f_values.push_back( t_value );
            }
            f_numerator_sum += t_numerator;
            f_denominator_sum += t_denominator;
        }

        // if points were found, recompute the center frequency and score
        if( t_found == true )
        {
            f_frequency = f_numerator_sum / f_denominator_sum;

            register real_t t_time;
            register real_t t_frequency;
            register real_t t_value;
            register real_t t_argument;
            register real_t t_scale = M_PI / (2. * s_spread);
            register real_t t_score = 0.;
            for( t_index = 0; t_index < f_values.size(); t_index++ )
            {
                t_time = f_times.at( t_index );
                t_frequency = f_frequencies.at( t_index );
                t_value = f_values.at( t_index );
                t_argument = (t_frequency - f_frequency) - s_slope * (t_time - f_time);
                t_score += t_value * (.5 + .5 * cos( t_scale * t_argument ));
            }

            f_add_score_sum = s_add_coefficient * pow( t_score, s_add_power );
            f_gap_score_sum += f_gap_score;
            f_gap_count = 0.;
            f_gap_score = 0.;
        }
        else
        {
            f_gap_count += 1.;
            f_gap_score = s_gap_coefficient * pow( f_gap_count, s_gap_power );
        }

        f_score = f_add_score_sum - f_gap_score_sum - f_gap_score;
        return;
    }

    const count_t& rf_candidate_consumer::cluster::id()
    {
        return f_id;
    }
    const real_t& rf_candidate_consumer::cluster::score()
    {
        return f_score;
    }

    const vector< real_t >& rf_candidate_consumer::cluster::times() const
    {
        return f_times;
    }
    const vector< real_t >& rf_candidate_consumer::cluster::frequencies() const
    {
        return f_frequencies;
    }
    const vector< real_t >& rf_candidate_consumer::cluster::values() const
    {
        return f_values;
    }

}

