#include "plot.hh"

#include "core_message.hh"

#include <limits>
using std::numeric_limits;

namespace midge
{

    plot::plot() :
            f_count( 0 ),
            f_plotters(),
            f_application( NULL )
    {

    }
    plot::~plot()
    {
        for( vector< plotter* >::iterator t_it = f_plotters.begin(); t_it != f_plotters.end(); ++t_it )
        {
            delete *t_it;
        }
        delete f_application;
    }
    void plot::initialize()
    {
        if( f_count++ == 0 )
        {
            f_application = new TApplication( "", 0, NULL );
        }
        return;
    }
    void plot::plot_time_series( const string& p_label, const string& p_title, const string& p_time_axis, const string& p_value_axis, const vector< real_t >& p_time_coordinates, const vector< real_t >& p_value_coordinates )
    {
        f_plotters.push_back( new time_plotter( p_label, p_title, p_time_axis, p_value_axis, p_time_coordinates, p_value_coordinates ) );
        return;
    }
    void plot::plot_frequency_series( const string& p_label, const string& p_title, const string& p_frequency_axis, const string& p_value_axis, const vector< real_t >& p_frequency_coordinates, const vector< real_t >& p_value_coordinates )
    {
        f_plotters.push_back( new frequency_plotter( p_label, p_title, p_frequency_axis, p_value_axis, p_frequency_coordinates, p_value_coordinates ) );
        return;
    }
    void plot::plot_time_frequency_series( const string& p_label, const string& p_title, const string& p_time_axis, const string& p_frequency_axis, const string& p_value_axis, const vector< real_t >& p_time_coordinates, const vector< real_t >& p_frequency_coordinates, const vector< real_t >& p_value_coordinates )
    {
        f_plotters.push_back( new time_frequency_plotter( p_label, p_title, p_time_axis, p_frequency_axis, p_value_axis, p_time_coordinates, p_frequency_coordinates, p_value_coordinates ) );
        return;
    }
    void plot::finalize()
    {
        if( (--f_count == 0) && (f_plotters.size() != 0) )
        {
            f_application->Run( kTRUE );
        }
        return;
    }

    plot::plotter::plotter()
    {
    }
    plot::plotter::~plotter()
    {
    }

    plot::time_plotter::time_plotter( const string& p_label, const string& p_title, const string& p_time_axis, const string& p_value_axis, const vector< real_t >& p_time_coordinates, const vector< real_t >& p_value_coordinates ) :
            f_canvas( NULL ),
            f_histogram( NULL )
    {
        if( p_value_coordinates.size() != p_time_coordinates.size() )
        {
            throw error() << "time plotter was given value coordinates with length <" << p_value_coordinates.size() << "> and time coordinates with size <" << p_time_coordinates.size() << ">";
        }

        msg_normal( coremsg, "making time plot <" << p_label << ">" << eom );

        count_t t_time_count = 0;
        real_t t_time_current = -1.;
        for( data_cit t_time_it = p_time_coordinates.begin(); t_time_it != p_time_coordinates.end(); ++t_time_it )
        {
            if( *t_time_it > t_time_current )
            {
                t_time_current = *t_time_it;
                t_time_count++;
            }
        }
        real_t t_time_increment = t_time_current / t_time_count;

        f_canvas = new TCanvas( (p_label + string( "_canvas" )).c_str(), (p_label + string( "_canvas" )).c_str(), 0, 0, 1024, 768 );
        f_histogram = new TH1D( (p_label + string( "_histogram" )).c_str(), (p_label + string( "_histogram" )).c_str(), t_time_count, 0. - .5 * t_time_increment, t_time_current + .5 * t_time_increment );
        f_histogram->SetDirectory( NULL );
        for( count_t t_index = 0; t_index < p_value_coordinates.size(); t_index++ )
        {
            f_histogram->Fill( p_time_coordinates[ t_index ], p_value_coordinates[ t_index ] );
        }

        f_canvas->cd( 0 );
        f_histogram->SetStats( kFALSE );
        f_histogram->SetTitle( p_title.c_str() );
        f_histogram->GetXaxis()->SetTitle( p_time_axis.c_str() );
        f_histogram->GetYaxis()->SetTitle( p_value_axis.c_str() );
        f_histogram->Draw( "LP" );

        return;
    }
    plot::time_plotter::~time_plotter()
    {
        delete f_canvas;
        delete f_histogram;
    }

    plot::frequency_plotter::frequency_plotter( const string& p_label, const string& p_title, const string& p_frequency_axis, const string& p_value_axis, const vector< real_t >& p_frequency_coordinates, const vector< real_t >& p_value_coordinates ) :
            f_canvas( NULL ),
            f_histogram( NULL )
    {
        if( p_value_coordinates.size() != p_frequency_coordinates.size() )
        {
            throw error() << "frequency plotter was given value coordinates with length <" << p_value_coordinates.size() << "> and frequency coordinates with size <" << p_frequency_coordinates.size() << ">";
        }

        msg_normal( coremsg, "making frequency plot <" << p_label << ">" << eom );

        count_t t_frequency_count = 0;
        real_t t_frequency_current = -1.;
        for( data_cit t_frequency_it = p_frequency_coordinates.begin(); t_frequency_it != p_frequency_coordinates.end(); ++t_frequency_it )
        {
            if( *t_frequency_it > t_frequency_current )
            {
                t_frequency_current = *t_frequency_it;
                t_frequency_count++;
            }
        }
        real_t t_frequency_increment = t_frequency_current / t_frequency_count;

        f_canvas = new TCanvas( (p_label + string( "_canvas" )).c_str(), (p_label + string( "_canvas" )).c_str(), 0, 0, 1024, 768 );
        f_histogram = new TH1D( (p_label + string( "_histogram" )).c_str(), (p_label + string( "_histogram" )).c_str(), t_frequency_count, 0. - .5 * t_frequency_increment, t_frequency_current + .5 * t_frequency_increment );
        f_histogram->SetDirectory( NULL );
        for( count_t t_index = 0; t_index < p_value_coordinates.size(); t_index++ )
        {
            f_histogram->Fill( p_frequency_coordinates[ t_index ], p_value_coordinates[ t_index ] );
        }

        f_canvas->cd( 0 );
        f_histogram->SetStats( kFALSE );
        f_histogram->SetTitle( p_title.c_str() );
        f_histogram->GetXaxis()->SetTitle( p_frequency_axis.c_str() );
        f_histogram->GetYaxis()->SetTitle( p_value_axis.c_str() );
        f_histogram->Draw( "LP" );

        return;
    }
    plot::frequency_plotter::~frequency_plotter()
    {
        delete f_canvas;
        delete f_histogram;
    }

    plot::time_frequency_plotter::time_frequency_plotter( const string& p_label, const string& p_title, const string& p_time_axis, const string& p_frequency_axis, const string& p_value_axis, const vector< real_t >& p_time_coordinates, const vector< real_t >& p_frequency_coordinates, const vector< real_t >& p_value_coordinates ) :
            f_canvas( NULL ),
            f_histogram( NULL )
    {
        if( p_value_coordinates.size() != p_time_coordinates.size() )
        {
            throw error() << "time frequency plotter was given value coordinates with length <" << p_value_coordinates.size() << "> and time coordinates with size <" << p_time_coordinates.size() << ">";
        }

        if( p_value_coordinates.size() != p_frequency_coordinates.size() )
        {
            throw error() << "time frequency plotter was given value coordinates with length <" << p_value_coordinates.size() << "> and frequency coordinates with size <" << p_frequency_coordinates.size() << ">";
        }

        msg_normal( coremsg, "making time frequency plot <" << p_label << ">" << eom );

        count_t t_time_count = 1;
        real_t t_time_first = p_time_coordinates.front();
        real_t t_time_last = p_time_coordinates.front();
        for( data_cit t_time_it = p_time_coordinates.begin(); t_time_it != p_time_coordinates.end(); ++t_time_it )
        {
            if( *t_time_it > t_time_last )
            {
                t_time_last = *t_time_it;
                t_time_count++;
            }
        }
        real_t t_time_increment = (t_time_last - t_time_first) / t_time_count;

        count_t t_frequency_count = 1;
        real_t t_frequency_first = p_frequency_coordinates.front();
        real_t t_frequency_last = p_frequency_coordinates.front();
        for( data_cit t_frequency_it = p_frequency_coordinates.begin(); t_frequency_it != p_frequency_coordinates.end(); ++t_frequency_it )
        {
            if( *t_frequency_it > t_frequency_last )
            {
                t_frequency_last = *t_frequency_it;
                t_frequency_count++;
            }
        }
        real_t t_frequency_increment = t_frequency_last / t_frequency_count;

        f_canvas = new TCanvas( (p_label + string( "_canvas" )).c_str(), (p_label + string( "_canvas" )).c_str(), 0, 0, 1024, 768 );
        f_histogram = new TH2D( (p_label + string( "_histogram" )).c_str(), (p_label + string( "_histogram" )).c_str(), t_time_count, t_time_first - .5 * t_time_increment, t_time_last + .5 * t_time_increment, t_frequency_count, t_frequency_first - .5 * t_frequency_increment, t_frequency_last + .5 * t_frequency_increment );
        f_histogram->SetDirectory( NULL );
        for( count_t t_index = 0; t_index < p_value_coordinates.size(); t_index++ )
        {
            f_histogram->Fill( p_time_coordinates[ t_index ], p_frequency_coordinates[ t_index ], p_value_coordinates[ t_index ] );
        }

        f_canvas->cd( 0 );
        f_histogram->SetStats( kFALSE );
        f_histogram->SetTitle( p_title.c_str() );
        f_histogram->GetXaxis()->SetTitle( p_time_axis.c_str() );
        f_histogram->GetYaxis()->SetTitle( p_frequency_axis.c_str() );
        f_histogram->GetZaxis()->SetTitle( p_value_axis.c_str() );
        f_histogram->Draw( "COLZ" );

        return;
    }
    plot::time_frequency_plotter::~time_frequency_plotter()
    {
        delete f_canvas;
        delete f_histogram;
    }
}
