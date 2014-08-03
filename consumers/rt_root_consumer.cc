#include "rt_root_consumer.hh"

#include "plot.hh"

#include <cmath>

namespace midge
{

    rt_root_consumer::rt_root_consumer() :
            f_file( "" ),
            f_plot( false ),
            f_plot_title( "" ),
            f_axis_title( "" ),
            f_stream( NULL ),
            f_tree( NULL ),
            f_time( 0. ),
            f_value( 0. ),
            f_size( 0 ),
            f_interval( 1. ),
            f_in( NULL ),
            f_index( 0 )
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

    void rt_root_consumer::set_plot_title( const string& p_plot_title )
    {
        f_plot_title = p_plot_title;
        return;
    }
    const string& rt_root_consumer::get_plot_title() const
    {
        return f_plot_title;
    }

    void rt_root_consumer::set_axis_title( const string& p_axis_title )
    {
        f_axis_title = p_axis_title;
        return;
    }
    const string& rt_root_consumer::get_axis_title() const
    {
        return f_axis_title;
    }

    bool rt_root_consumer::start_consumer()
    {
        if( f_plot == true )
        {
            plot::get_instance()->initialize();
        }

        f_stream = new TFile( f_file.c_str(), "RECREATE" );
        f_tree = new TTree( get_name().c_str(), get_name().c_str() );
        f_tree->SetDirectory( f_stream );
        f_tree->Branch( "time", &f_time, 64000, 99 );
        f_tree->Branch( "value", &f_value, 64000, 99 );

        f_size = in< 0 >()->get_size();
        f_interval = in< 0 >()->get_interval();
        f_in = in< 0 >()->raw();
        f_index = 0.;

        return true;
    }

    bool rt_root_consumer::execute_consumer()
    {
        count_t t_index;

        count_t t_next = (count_t) (round( in< 0 >()->get_time() / f_interval ));

        if( t_next < f_index )
        {
            for( t_index = f_index; t_index < t_next + f_size; t_index++ )
            {
                f_time = t_index * f_interval;
                f_value = f_in[ t_index - t_next ];
                f_tree->Fill();
            }
        }
        else
        {
            for( t_index = t_next; t_index < t_next + f_size; t_index++ )
            {
                f_time = t_index * f_interval;
                f_value = f_in[ t_index - t_next ];
                f_tree->Fill();
            }
        }

        f_index = t_next + f_size;

        return true;
    }

    bool rt_root_consumer::stop_consumer()
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
            Long64_t t_entries = t_tree->GetEntries();
            real_t t_time;
            real_t t_value;
            t_tree->SetBranchAddress( "time", &t_time );
            t_tree->SetBranchAddress( "value", &t_value );

            plot::abscissa t_times( t_entries );
            t_times.title() = string( "Time [sec]" );
            t_times.count() = f_index;
            t_times.low() = 0.;
            t_times.high() = (f_index - 1) * f_interval;

            plot::ordinate t_values( t_entries );
            t_values.title() = f_axis_title;

            for( Long64_t t_index = 0; t_index < t_entries; t_index++ )
            {
                t_tree->GetEntry( t_index );

                t_times.values().at( t_index ) = t_time;
                t_values.values().at( t_index ) = t_value;
            }

            t_file->Close();
            delete t_file;

            plot::get_instance()->plot_one_dimensional( get_name(), f_plot_title, t_times, t_values );

            plot::get_instance()->finalize();
        }

        f_size = 0;
        f_interval = 1.;
        f_in = NULL;
        f_index = 0.;

        return true;
    }

}
