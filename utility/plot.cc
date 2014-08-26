#include "plot.hh"
#include "error.hh"
#include "message.hh"

#include <limits>
using std::numeric_limits;

namespace midge
{

    plot::ordinate::ordinate() :
            f_title( "" ),
            f_values()
    {
    }
    plot::ordinate::ordinate( const count_t& p_size ) :
            f_title( "" ),
            f_values( p_size, 0. )
    {
    }
    plot::ordinate::~ordinate()
    {
    }

    void plot::ordinate::operator()( const count_t& p_size )
    {
        f_values.clear();
        f_values.resize( p_size, 0. );
    }

    string& plot::ordinate::title()
    {
        return f_title;
    }
    const string& plot::ordinate::title() const
    {
        return f_title;
    }

    plot::values_t& plot::ordinate::values()
    {
        return f_values;
    }
    const plot::values_t& plot::ordinate::values() const
    {
        return f_values;
    }

    plot::abscissa::abscissa() :
            f_title( "" ),
            f_count( 0 ),
            f_low( 0. ),
            f_high( 0. ),
            f_values()
    {
    }
    plot::abscissa::abscissa( const count_t& p_size ) :
            f_title( "" ),
            f_count( 0 ),
            f_low( 0. ),
            f_high( 0. ),
            f_values( p_size, 0. )
    {
    }
    plot::abscissa::~abscissa()
    {
    }

    void plot::abscissa::operator()( const count_t& p_size )
    {
        f_values.clear();
        f_values.resize( p_size, 0. );
    }

    string& plot::abscissa::title()
    {
        return f_title;
    }
    const string& plot::abscissa::title() const
    {
        return f_title;
    }

    count_t& plot::abscissa::count()
    {
        return f_count;
    }
    const count_t& plot::abscissa::count() const
    {
        return f_count;
    }

    real_t& plot::abscissa::low()
    {
        return f_low;
    }
    const real_t& plot::abscissa::low() const
    {
        return f_low;
    }

    real_t& plot::abscissa::high()
    {
        return f_high;
    }
    const real_t& plot::abscissa::high() const
    {
        return f_high;
    }

    plot::values_t& plot::abscissa::values()
    {
        return f_values;
    }
    const plot::values_t& plot::abscissa::values() const
    {
        return f_values;
    }

    plot::plot() :
            f_count( 0 ),
            f_application( NULL ),
            f_plots(),
            f_th1s(),
            f_th2s(),
            f_graphs()
    {

    }
    plot::~plot()
    {
        for( plot_it t_it = f_plots.begin(); t_it != f_plots.end(); ++t_it )
        {
            delete t_it->second.first;
        }
        for( th1_it t_it = f_th1s.begin(); t_it != f_th1s.end(); ++t_it )
        {
            delete t_it->second;
        }
        for( th2_it t_it = f_th2s.begin(); t_it != f_th2s.end(); ++t_it )
        {
            delete t_it->second;
        }
        for( graph_it t_it = f_graphs.begin(); t_it != f_graphs.end(); ++t_it )
        {
            delete t_it->second;
        }
        delete f_application;
    }
    void plot::initialize()
    {
        f_mutex.lock();

        if( (f_count++ == 0) && (f_application == NULL) )
        {
            f_application = new TApplication( "", 0, NULL );

            const count_t t_points = 5;
            const count_t t_contours = 255;

            real_t t_stops[ t_points ] =
            { 0.00, 0.34, 0.61, 0.84, 1.00 };

            real_t t_red[ t_points ] =
            { 0.00, 0.00, 0.87, 1.00, 0.51 };

            real_t t_green[ t_points ] =
            { 0.00, 0.81, 1.00, 0.20, 0.00 };

            real_t t_blue[ t_points ] =
            { 0.51, 1.00, 0.12, 0.00, 0.00 };

            TColor::CreateGradientColorTable( t_points, t_stops, t_red, t_green, t_blue, t_contours );
            gStyle->SetNumberContours( t_contours );
        }

        f_mutex.unlock();

        return;
    }
    void plot::plot_one_dimensional( const string& p_key, const string& p_name, const string& p_title, const abscissa& p_x, const ordinate& p_y )
    {
        f_mutex.lock();

        if( p_x.values().size() != p_y.values().size() )
        {
            throw error() << "plot one dimensional was given x values with size <" << p_x.values().size() << "> and y values with size <" << p_y.values().size() << ">";
        }

        msg_normal( msg, "making one dimensional plot <" << p_key << "> on canvas <" << p_key << ">" << eom );

        real_t t_x_increment = (p_x.high() - p_x.low()) / p_x.count();

        TH1D* t_histogram;
        th1_it t_hist_it = f_th1s.find( p_name );
        if( t_hist_it != f_th1s.end() )
        {
            t_histogram = t_hist_it->second;
        }
        else
        {
            t_histogram = new TH1D( p_name.c_str(), p_name.c_str(), p_x.count(), p_x.low() - .5 * t_x_increment, p_x.high() + .5 * t_x_increment );
            t_histogram->SetDirectory( NULL );
            f_th1s.insert( th1_entry( p_name, t_histogram ) );
        }

        for( count_t t_index = 0; t_index < p_x.values().size(); t_index++ )
        {
            if( p_y.values().at( t_index ) != p_y.values().at( t_index ) )
            {
                continue;
            }
            t_histogram->Fill( p_x.values().at( t_index ), p_y.values().at( t_index ) );
        }
        real_t t_y_low = t_histogram->GetMinimum();
        real_t t_y_high = t_histogram->GetMaximum();
        real_t t_y_increment = (t_y_high - t_y_low) / 8.;

        TCanvas* t_canvas;
        TH1* t_axes;
        plot_it t_plot_it = f_plots.find( p_key );
        if( t_plot_it != f_plots.end() )
        {
            t_canvas = t_plot_it->second.first;
            t_canvas->cd( 0 );
            t_axes = t_plot_it->second.second;
            t_axes->SetTitle( p_title.c_str() );
            t_axes->GetXaxis()->SetTitle( p_x.title().c_str() );
            t_axes->GetYaxis()->SetTitle( p_y.title().c_str() );
            t_histogram->SetStats( kFALSE );
            t_histogram->Draw( "LP SAME" );
        }
        else
        {
            t_canvas = new TCanvas( p_key.c_str(), p_key.c_str(), 0, 0, 1024, 768 );
            t_canvas->cd( 0 );
            t_axes = t_canvas->DrawFrame( p_x.low() - .5 * t_x_increment, t_y_low - .5 * t_y_increment, p_x.high() + .5 * t_x_increment, t_y_high + .5 * t_y_increment );
            t_histogram->SetStats( kFALSE );
            t_histogram->SetTitle( p_title.c_str() );
            t_histogram->GetXaxis()->SetTitle( p_x.title().c_str() );
            t_histogram->GetYaxis()->SetTitle( p_y.title().c_str() );
            t_histogram->Draw( "LP" );
            f_plots.insert( plot_entry( p_key, plot_pair( t_canvas, t_axes ) ) );
        }

        f_mutex.unlock();

        return;
    }
    void plot::plot_two_dimensional( const string& p_key, const string& p_name, const string& p_title, const abscissa& p_x, const abscissa& p_y, const ordinate& p_z )
    {
        f_mutex.lock();

        if( p_z.values().size() != p_x.values().size() )
        {
            throw error() << "time frequency plotter was given z values with size <" << p_z.values().size() << "> and x values with size <" << p_x.values().size() << ">";
        }

        if( p_z.values().size() != p_y.values().size() )
        {
            throw error() << "time frequency plotter was given z values with size <" << p_z.values().size() << "> and y values with size <" << p_y.values().size() << ">";
        }

        msg_normal( msg, "making two dimensional plot <" << p_name << "> on canvas <" << p_key << ">" << eom );

        real_t t_x_increment = (p_x.high() - p_x.low()) / p_x.count();
        real_t t_y_increment = (p_y.high() - p_y.low()) / p_y.count();

        TH2D* t_histogram;
        th2_it t_hist_it = f_th2s.find( p_name );
        if( t_hist_it != f_th2s.end() )
        {
            t_histogram = t_hist_it->second;
        }
        else
        {
            t_histogram = new TH2D( p_name.c_str(), p_name.c_str(), p_x.count(), p_x.low() - .5 * t_x_increment, p_x.high() + .5 * t_x_increment, p_y.count(), p_y.low() - .5 * t_y_increment, p_y.high() + .5 * t_y_increment );
            t_histogram->SetDirectory( NULL );
            f_th2s.insert( th2_entry( p_name, t_histogram ) );
        }

        for( count_t t_index = 0; t_index < p_z.values().size(); t_index++ )
        {
            if( p_z.values().at( t_index ) != p_z.values().at( t_index ) )
            {
                continue;
            }
            t_histogram->Fill( p_x.values().at( t_index ), p_y.values().at( t_index ), p_z.values().at( t_index ) );
        }

        TCanvas* t_canvas;
        TH1* t_axes;
        plot_it t_plot_it = f_plots.find( p_key );
        if( t_plot_it != f_plots.end() )
        {
            t_canvas = t_plot_it->second.first;
            t_canvas->cd( 0 );
            t_axes = t_plot_it->second.second;
            t_axes->SetTitle( p_title.c_str() );
            t_axes->GetXaxis()->SetTitle( p_x.title().c_str() );
            t_axes->GetYaxis()->SetTitle( p_y.title().c_str() );
            t_axes->GetZaxis()->SetTitle( p_z.title().c_str() );
            t_histogram->SetStats( kFALSE );
            t_histogram->Draw( "COL SAME" );
        }
        else
        {
            t_canvas = new TCanvas( p_key.c_str(), p_key.c_str(), 0, 0, 1024, 768 );
            t_canvas->cd( 0 );
            t_axes = t_canvas->DrawFrame( p_x.low() - .5 * t_x_increment, p_y.low() - .5 * t_y_increment, p_x.high() + .5 * t_x_increment, p_y.high() + .5 * t_y_increment );
            t_axes->SetTitle( p_title.c_str() );
            t_axes->GetXaxis()->SetTitle( p_x.title().c_str() );
            t_axes->GetYaxis()->SetTitle( p_y.title().c_str() );
            t_axes->GetZaxis()->SetTitle( p_z.title().c_str() );
            t_histogram->SetStats( kFALSE );
            t_histogram->Draw( "COLZ SAME" );
            f_plots.insert( plot_entry( p_key, plot_pair( t_canvas, t_axes ) ) );
        }

        f_mutex.unlock();

        return;
    }
    void plot::graph_two_dimensional( const string& p_key, const string& p_name, const string& p_title, const abscissa& p_x, const abscissa& p_y )
    {
        f_mutex.lock();

        if( p_y.values().size() != p_x.values().size() )
        {
            throw error() << "time frequency plotter was given z values with size <" << p_y.values().size() << "> and x values with size <" << p_x.values().size() << ">";
        }

        msg_normal( msg, "making two dimensional graph <" << p_name << "> on canvas <" << p_key << ">" << eom );

        real_t t_x_increment = (p_x.high() - p_x.low()) / p_x.count();
        real_t t_y_increment = (p_y.high() - p_y.low()) / p_y.count();

        TGraph* t_graph = new TGraph();
        f_graphs.insert( graph_entry( p_name, t_graph ) );

        for( count_t t_index = 0; t_index < p_y.values().size(); t_index++ )
        {
            if( p_y.values().at( t_index ) != p_y.values().at( t_index ) )
            {
                continue;
            }
            t_graph->SetPoint( t_graph->GetN(), p_x.values().at( t_index ), p_y.values().at( t_index ) );
        }

        TCanvas* t_canvas;
        TH1* t_axes;
        plot_it t_plot_it = f_plots.find( p_key );
        if( t_plot_it != f_plots.end() )
        {
            t_canvas = t_plot_it->second.first;
            t_canvas->cd( 0 );
            t_axes = t_plot_it->second.second;
            t_axes->SetTitle( p_title.c_str() );
            t_axes->GetXaxis()->SetTitle( p_x.title().c_str() );
            t_axes->GetYaxis()->SetTitle( p_y.title().c_str() );
            t_graph->Draw( "L" );
        }
        else
        {
            t_canvas = new TCanvas( p_key.c_str(), p_key.c_str(), 0, 0, 1024, 768 );
            t_canvas->cd( 0 );
            t_axes = t_canvas->DrawFrame( p_x.low() - .5 * t_x_increment, p_y.low() - .5 * t_y_increment, p_x.high() + .5 * t_x_increment, p_y.high() + .5 * t_y_increment );
            t_axes->SetTitle( p_title.c_str() );
            t_axes->GetXaxis()->SetTitle( p_x.title().c_str() );
            t_axes->GetYaxis()->SetTitle( p_y.title().c_str() );
            t_graph->Draw( "L" );
            f_plots.insert( plot_entry( p_key, plot_pair( t_canvas, t_axes ) ) );
        }

        f_mutex.unlock();

        return;
    }
    void plot::finalize()
    {
        f_mutex.lock();

        if( (--f_count == 0) && (f_plots.size() != 0) )
        {
            f_application->Run( kTRUE );
        }

        f_mutex.unlock();

        return;
    }
}
