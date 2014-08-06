#include "rf_rf_threshold_transformer.hh"

#include "plot.hh"

#include <limits>
using std::numeric_limits;

#include <cmath>

namespace midge
{

    rf_rf_threshold_transformer::rf_rf_threshold_transformer() :
            f_threshold_file( "" ),
            f_background_file( "" ),
            f_threshold( -1. ),
            f_minimum_frequency( numeric_limits< real_t >::min() ),
            f_maximum_frequency( numeric_limits< real_t >::max() ),
            f_plot( false ),
            f_plot_key( "" ),
            f_plot_name( "" ),
            f_chart_title( "" ),
            f_axis_title( "" ),
            f_stream( NULL ),
            f_label( NULL ),
            f_tree( NULL ),
            f_time( 0. ),
            f_frequency( 0. ),
            f_value( 0. ),
            f_size( 0 ),
            f_interval( 1. ),
            f_in( NULL ),
            f_out( NULL ),
            f_background( NULL ),
            f_minimum_frequency_index( 0 ),
            f_maximum_frequency_index( numeric_limits< count_t >::max() ),
            f_first_time( 0. ),
            f_last_time( 0. ),
            f_count( 0 )
    {
    }
    rf_rf_threshold_transformer::~rf_rf_threshold_transformer()
    {
    }

    bool rf_rf_threshold_transformer::start_transformer()
    {
        if( f_plot == true )
        {
            plot::get_instance()->initialize();
        }

        f_size = in< 0 >()->get_size();
        f_interval = in< 0 >()->get_interval();

        out< 0 >()->set_size( f_size );
        out< 0 >()->set_interval( f_interval );

        f_in = in< 0 >()->raw();
        f_out = out< 0 >()->raw();

        if( f_threshold_file.size() == 0 )
        {
            throw error() << "cannot start rf rf threshold transformer <" << get_name() << "> with no candidate file set";
        }

        f_stream = new TFile( f_threshold_file.c_str(), "RECREATE" );

        f_label = new TObjString( "midge_threshold" );

        f_tree = new TTree( get_name().c_str(), get_name().c_str() );
        f_tree->SetDirectory( f_stream );
        f_tree->Branch( "time", &f_time );
        f_tree->Branch( "frequency", &f_frequency );
        f_tree->Branch( "value", &f_value );

        f_background = new real_t[ f_size ];

        if( f_background_file.size() == 0 )
        {
            throw error() << "cannot start rf rf threshold transformer <" << get_name() << "> with no background file set";
        }

        TFile* t_stream = new TFile( f_background_file.c_str(), "READ" );
        if( t_stream->IsZombie() == true )
        {
            throw error() << "cannot read background file";
        }

        TObjString* t_label = (TObjString*) (t_stream->Get( "midge_background" ) );
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

        f_minimum_frequency_index = (count_t) (round( f_minimum_frequency / f_interval ));
        f_maximum_frequency_index = (count_t) (round( f_maximum_frequency / f_interval ));
        f_first_time = 0.;
        f_last_time = 0.;
        f_count = 0;

        return true;
    }

    bool rf_rf_threshold_transformer::execute_transformer()
    {
        if( f_count == 0 )
        {
            f_first_time = in< 0 >()->get_time();
        }
        else
        {
            f_last_time = in< 0 >()->get_time();
        }
        f_count++;

        register double t_in;
        register double t_background;
        register double t_value;
        for( count_t t_index = f_minimum_frequency_index; t_index <= f_maximum_frequency_index; t_index++ )
        {
            t_in = f_in[ t_index ];
            t_background = f_background[ t_index ];
            t_value = (t_in - t_background) / t_background;
            if( t_value < f_threshold )
            {
                t_value = numeric_limits< real_t >::quiet_NaN();
            }
            f_out[ t_index ] = t_value;

            f_time = in< 0 >()->get_time();
            f_frequency = f_interval * t_index;
            f_value = t_value;
            f_tree->Fill();
        }

        return true;
    }

    bool rf_rf_threshold_transformer::stop_transformer()
    {
        f_stream->cd();
        f_label->Write();
        f_tree->Write();
        f_stream->Close();
        delete f_stream;

        if( f_plot == true )
        {
            TFile* t_file = new TFile( f_threshold_file.c_str(), "READ" );

            TTree* t_tree = (TTree*) (t_file->Get( get_name().c_str() ));
            Long64_t t_entries = t_tree->GetEntries();
            real_t t_time;
            real_t t_frequency;
            real_t t_value;
            t_tree->SetBranchAddress( "time", &t_time );
            t_tree->SetBranchAddress( "frequency", &t_frequency );
            t_tree->SetBranchAddress( "value", &t_value );

            plot::abscissa t_times( t_entries );
            t_times.title() = string( "Time [sec]" );
            t_times.count() = f_count;
            t_times.low() = f_first_time;
            t_times.high() = f_last_time;

            plot::abscissa t_frequencies( t_entries );
            t_frequencies.title() = string( "Frequency [Hz]" );
            t_frequencies.count() = f_maximum_frequency_index - f_minimum_frequency_index + 1;
            t_frequencies.low() = f_minimum_frequency_index * f_interval;
            t_frequencies.high() = f_maximum_frequency_index * f_interval;

            plot::ordinate t_values( t_entries );
            t_values.title() = f_axis_title;

            for( Long64_t t_index = 0; t_index < t_entries; t_index++ )
            {
                t_tree->GetEntry( t_index );

                t_times.values().at( t_index ) = t_time;
                t_frequencies.values().at( t_index ) = t_frequency;
                t_values.values().at( t_index ) = t_value;
            }

            t_file->Close();
            delete t_file;

            plot::get_instance()->plot_two_dimensional( f_plot_key, f_plot_name, f_chart_title, t_times, t_frequencies, t_values );

            plot::get_instance()->finalize();
        }

        f_size = 0;
        f_interval = 1.;
        f_in = NULL;
        f_out = NULL;

        delete[] f_background;
        f_background = NULL;

        f_minimum_frequency_index = 0;
        f_maximum_frequency_index = numeric_limits< count_t >::max();
        f_first_time = 0.;
        f_last_time = 0.;
        f_count = 0;

        return true;
    }

}

