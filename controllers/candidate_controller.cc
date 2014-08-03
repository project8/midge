#include "candidate_controller.hh"

#include "plot.hh"

#include <limits>
using std::numeric_limits;

#include <cmath>

namespace midge
{

    candidate_controller::candidate_controller() :
            f_candidate_file( "" ),
            f_background_file( "" ),
            f_background_name( "" ),
            f_rough_stride( 8192 ),
            f_fine_stride( 512 ),
            f_threshold_ratio( 1. ),
            f_minimum_time( numeric_limits< real_t >::min() ),
            f_maximum_time( numeric_limits< real_t >::max() ),
            f_minimum_frequency( numeric_limits< real_t >::min() ),
            f_maximum_frequency( numeric_limits< real_t >::max() ),
            f_minimum_slope( numeric_limits< real_t >::max() ),
            f_maximum_slope( numeric_limits< real_t >::min() ),
            f_plot( false ),
            f_plot_title( "" ),
            f_axis_title( "" ),
            f_signal_size( 0 ),
            f_signal_interval( 1. ),
            f_signal( NULL ),
            f_power_frequency_size( 0 ),
            f_power_frequency_interval( 1. ),
            f_power( NULL ),
            f_background( NULL ),
            f_minimum_time_index( 0 ),
            f_maximum_time_index( 0 ),
            f_minimum_frequency_index( 0 ),
            f_maximum_frequency_index( 0 ),
            f_minimum_slope_value( 0. ),
            f_maximum_slope_value( 0. ),
            f_stream( NULL ),
            f_tree( NULL ),
            f_time( 0. ),
            f_frequency( 0. ),
            f_value( 0. )
    {
    }
    candidate_controller::~candidate_controller()
    {
    }

    bool candidate_controller::start_controller_pre()
    {
        f_stream = new TFile( f_candidate_file.c_str(), "RECREATE" );
        f_tree = new TTree( get_name().c_str(), get_name().c_str() );
        f_tree->SetDirectory( f_stream );
        f_tree->Branch( "time", &f_time );
        f_tree->Branch( "frequency", &f_frequency );
        f_tree->Branch( "value", &f_value );

        return true;
    }

    bool candidate_controller::start_controller_post()
    {
        if( f_plot == true )
        {
            plot::get_instance()->initialize();
        }

        f_signal_size = in< 0 >()->get_size();
        f_signal_interval = in< 0 >()->get_interval();
        f_signal = in< 0 >()->raw();

        f_power_frequency_size = in< 1 >()->get_size();
        f_power_frequency_interval = in< 1 >()->get_interval();
        f_power = in< 1 >()->raw();

        f_background = new real_t[ f_power_frequency_size ];

        f_minimum_time_index = (count_t) (round( f_minimum_time / (f_signal_interval * f_signal_size) ) * f_signal_size);
        f_maximum_time_index = (count_t) (round( f_maximum_time / (f_signal_interval * f_signal_size) ) * f_signal_size);
        f_minimum_frequency_index = (count_t) (round( f_minimum_frequency / f_power_frequency_interval ));
        f_maximum_frequency_index = (count_t) (round( f_maximum_frequency / f_power_frequency_interval ));
        f_minimum_slope_value = f_signal_size * f_minimum_slope;
        f_maximum_slope_value = f_signal_size * f_maximum_slope;

        if( f_background_file.size() == 0 )
        {
            throw error() << "cannot start candidate controller <" << get_name() << "> with no background file set";
        }

        if( f_candidate_file.size() == 0 )
        {
            throw error() << "cannot start candidate controller <" << get_name() << "> with no candidate file set";
        }

        TFile* t_stream = new TFile( f_background_file.c_str(), "READ" );
        if( t_stream->IsZombie() == true )
        {
            throw error() << "cannot read background file <" << f_background_file << ">";
        }

        TTree* t_tree = (TTree*) (t_stream->Get( f_background_name.c_str() ));
        if( t_tree == NULL )
        {
            throw error() << "cannot read background tree <" << f_background_name << ">";
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
        if( t_size != f_power_frequency_size )
        {
            throw error() << "cannot analyze background tree with size <" << t_size << "> against input with size <" << f_power_frequency_size << ">";
        }

        for( Long64_t t_index = 0; t_index < t_size; t_index++ )
        {
            t_tree->GetEntry( t_index );
            f_background[ t_index ] = t_value;
        }

        t_stream->Close();
        delete t_stream;

        out< 0 >()->set_offset( f_minimum_time_index );

        return true;
    }

    bool candidate_controller::execute_controller_pre()
    {
        return true;
    }

    bool candidate_controller::execute_controller_post()
    {
        f_time = in< 0 >()->get_time();

        if( (count_t) (round( f_time / f_signal_interval )) == f_maximum_time_index )
        {
            return false;
        }

        register double t_in;
        register double t_background;
        register double t_value;
        for( count_t t_frequency_index = f_minimum_frequency_index; t_frequency_index <= f_maximum_frequency_index; t_frequency_index++ )
        {
            t_in = f_power[ t_frequency_index ];
            t_background = f_background[ t_frequency_index ];
            t_value = (t_in - t_background) / t_background;
            if( t_value > f_threshold_ratio )
            {
                f_value = t_value;
                f_frequency = f_power_frequency_interval * t_frequency_index;
                f_tree->Fill();
            }
        }

        out< 0 >()->set_stride( f_rough_stride );

        return true;
    }

    bool candidate_controller::stop_controller_pre()
    {
        f_stream->cd();
        f_tree->Write();
        f_stream->Close();
        delete f_stream;

        return true;
    }

    bool candidate_controller::stop_controller_post()
    {
        if( f_plot == true )
        {
            TFile* t_file = new TFile( f_candidate_file.c_str(), "READ" );

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
            t_times.count() = (f_maximum_time_index - f_minimum_time_index) / f_rough_stride;
            t_times.low() = f_minimum_time_index * f_signal_interval;
            t_times.high() = (f_maximum_time_index - f_signal_size) * f_signal_interval;

            plot::abscissa t_frequencies( t_entries );
            t_frequencies.title() = string( "Frequency [Hz]" );
            t_frequencies.count() = f_maximum_frequency_index - f_minimum_frequency_index + 1;
            t_frequencies.low() = f_minimum_frequency_index * f_power_frequency_interval;
            t_frequencies.high() = f_maximum_frequency_index * f_power_frequency_interval;

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

            plot::get_instance()->plot_two_dimensional( get_name(), f_plot_title, t_times, t_frequencies, t_values );

            plot::get_instance()->finalize();
        }

        f_power_frequency_size = 0;
        f_power_frequency_interval = 1.;
        f_power = NULL;
        delete[] f_background;
        f_background = NULL;

        return true;
    }

}

