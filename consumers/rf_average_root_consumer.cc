#include "rf_average_root_consumer.hh"

#include "plot.hh"

namespace midge
{

    rf_average_root_consumer::rf_average_root_consumer() :
            f_file( "" ),
            f_plot( false ),
            f_stream( NULL ),
            f_tree( NULL ),
            f_time_point( 0. ),
            f_frequency_point( 0. ),
            f_value_point( 0. ),
            f_size( 0 ),
            f_interval( 1. ),
            f_in( NULL ),
            f_count( 0 ),
            f_average( NULL )
    {
    }
    rf_average_root_consumer::~rf_average_root_consumer()
    {
    }

    void rf_average_root_consumer::set_file( const string& p_file )
    {
        f_file = p_file;
        return;
    }
    const string& rf_average_root_consumer::get_file() const
    {
        return f_file;
    }

    void rf_average_root_consumer::set_plot( const bool_t& p_plot )
    {
        f_plot = p_plot;
        return;
    }
    const bool_t& rf_average_root_consumer::get_plot() const
    {
        return f_plot;
    }

    void rf_average_root_consumer::initialize_consumer()
    {
        if( f_plot == true )
        {
            plot::get_instance()->initialize();
        }

        f_stream = new TFile( f_file.c_str(), "RECREATE" );

        f_tree = new TTree( get_name().c_str(), get_name().c_str() );
        f_tree->SetDirectory( f_stream );
        f_tree->Branch( "frequency", &f_frequency_point, 64000, 99 );
        f_tree->Branch( "value", &f_value_point, 64000, 99 );

        return;
    }

    bool rf_average_root_consumer::start_consumer()
    {
        f_size = in< 0 >()->get_size();
        f_interval = in< 0 >()->get_interval();
        f_in = in< 0 >()->raw();
        f_count = 0;
        f_average = new real_t[ f_size ];

        for( count_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_average[ t_index ] = 0.;
        }

        return true;
    }

    bool rf_average_root_consumer::execute_consumer()
    {
        for( count_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_average[ t_index ] += f_in[ t_index ];
        }
        f_count++;

        return true;
    }

    bool rf_average_root_consumer::stop_consumer()
    {
        for( count_t t_index = 0; t_index < f_size; t_index++ )
        {
            f_frequency_point = t_index * f_interval;
            f_value_point = f_average[ t_index ] / f_count;
            f_tree->Fill();
        }

        f_size = 0;
        f_interval = 1.;
        f_in = NULL;
        f_count = 0;
        delete[] f_average;

        return true;
    }

    void rf_average_root_consumer::finalize_consumer()
    {
        f_stream->cd();
        f_tree->Write();
        f_stream->Close();
        delete f_stream;

        if( f_plot == true )
        {
            TFile* t_file = new TFile( f_file.c_str(), "READ" );

            TTree* t_tree = (TTree*) (t_file->Get( get_name().c_str() ));
            Long64_t t_count = t_tree->GetEntries();
            real_t t_frequency;
            real_t t_value;
            t_tree->SetBranchAddress( "frequency", &t_frequency );
            t_tree->SetBranchAddress( "value", &t_value );

            vector< real_t > t_frequencies( t_count );
            vector< real_t > t_values( t_count );
            for( Long64_t t_index = 0; t_index < t_tree->GetEntries(); t_index++ )
            {
                t_tree->GetEntry( t_index );

                t_frequencies[ t_index ] = t_frequency;
                t_values[ t_index ] = t_value;
            }

            t_file->Close();
            delete t_file;

            plot::get_instance()->plot_frequency_series( get_name(), "Frequency Series", "Frequency [Hz]", "Values", t_frequencies, t_values );

            plot::get_instance()->finalize();
        }

        return;
    }
}
