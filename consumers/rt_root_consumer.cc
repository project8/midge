#include "rt_root_consumer.hh"

#include "plot.hh"

#include <cmath>

namespace midge
{

    rt_root_consumer::rt_root_consumer() :
            f_file( "" ),
            f_plot( false ),
            f_stream( NULL ),
            f_tree( NULL ),
            f_time_point( 0. ),
            f_value_point( 0. ),
            f_size( 0 ),
            f_interval( 1. ),
            f_in( NULL ),
            f_time( 0. )
    {
    }
    rt_root_consumer::~rt_root_consumer()
    {
    }

    void rt_root_consumer::set_file( const string& p_file )
    {
        f_file = p_file;
        return;
    }
    const string& rt_root_consumer::get_file() const
    {
        return f_file;
    }

    void rt_root_consumer::set_plot( const bool_t& p_plot )
    {
        f_plot = p_plot;
        return;
    }
    const bool_t& rt_root_consumer::get_plot() const
    {
        return f_plot;
    }

    void rt_root_consumer::initialize_consumer()
    {
        if( f_plot == true )
        {
            plot::get_instance()->initialize();
        }

        f_stream = new TFile( f_file.c_str(), "RECREATE" );

        f_tree = new TTree( get_name().c_str(), get_name().c_str() );
        f_tree->SetDirectory( f_stream );
        f_tree->Branch( "time", &f_time_point, 64000, 99 );
        f_tree->Branch( "value", &f_value_point, 64000, 99 );

        return;
    }

    bool rt_root_consumer::start_consumer()
    {
        f_size = in< 0 >()->get_size();
        f_interval = in< 0 >()->get_interval();
        f_in = in< 0 >()->raw();

        f_time = 0.;

        return true;
    }

    bool rt_root_consumer::execute_consumer()
    {
        real_t t_start_time = in< 0 >()->get_start_time();
        count_t t_start_index = (count_t) (round( t_start_time / f_interval ));
        real_t t_stop_time = in< 0 >()->get_start_time() + f_size * f_interval;
        count_t t_stop_index = (count_t) (round( t_stop_time / f_interval ));

        count_t t_current_index;
        if( t_start_time > f_time )
        {
            t_current_index = t_start_index;
        }
        else
        {
            t_current_index = (count_t) (round( f_time / f_interval ));
        }

        for( count_t t_index = t_current_index; t_index < t_stop_index; t_index++ )
        {
            f_time_point = (t_start_time + (t_index - t_start_index) * f_interval);
            f_value_point = f_in[ t_index - t_start_index ];
            f_tree->Fill();
        }

        f_time = t_stop_time;

        return true;
    }

    bool rt_root_consumer::stop_consumer()
    {
        f_size = 0;
        f_interval = 1.;
        f_in = NULL;

        f_time = 0.;

        return true;
    }

    void rt_root_consumer::finalize_consumer()
    {
        f_stream->cd();
        f_tree->Write();
        f_stream->Close();
        delete f_stream;

        if( f_plot == true )
        {
            msg_debug( coremsg, "  rt root consumer <" << get_name() << "> plotting" << eom );

            TFile* t_file = new TFile( f_file.c_str(), "READ" );

            TTree* t_tree = (TTree*) (t_file->Get( get_name().c_str() ));
            Long64_t t_count = t_tree->GetEntries();
            real_t t_time;
            real_t t_value;
            t_tree->SetBranchAddress( "time", &t_time );
            t_tree->SetBranchAddress( "value", &t_value );

            vector< real_t > t_times( t_count );
            vector< real_t > t_values( t_count );
            for( Long64_t t_index = 0; t_index < t_tree->GetEntries(); t_index++ )
            {
                t_tree->GetEntry( t_index );

                t_times[ t_index ] = t_time;
                t_values[ t_index ] = t_value;
            }

            t_file->Close();
            delete t_file;

            plot::get_instance()->plot_time_series( get_name(), "Time Frequency Series", "Time [sec]", "Values", t_times, t_values );

            plot::get_instance()->finalize();
        }

        return;
    }
}
