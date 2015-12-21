#include "../library/utility/plot.hh"

#include <limits>

#include "../library/utility/error.hh"
#include "../library/utility/message.hh"
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

    plot::plot_one_dimensional::plot_one_dimensional( const string_t& p_key, const string_t& p_name, const string_t& p_title, const abscissa& p_x_axis, const ordinate& p_y_values ) :
                f_key( p_key ),
                f_name( p_name ),
                f_title( p_title ),
                f_x_axis( p_x_axis ),
                f_y_values( p_y_values )
    {
    }
    plot::plot_one_dimensional::~plot_one_dimensional()
    {
    }

    string& plot::plot_one_dimensional::key()
    {
        return f_key;
    }
    const string& plot::plot_one_dimensional::key() const
    {
        return f_key;
    }

    string& plot::plot_one_dimensional::name()
    {
        return f_name;
    }
    const string& plot::plot_one_dimensional::name() const
    {
        return f_name;
    }

    string& plot::plot_one_dimensional::title()
    {
        return f_title;
    }
    const string& plot::plot_one_dimensional::title() const
    {
        return f_title;
    }

    plot::abscissa& plot::plot_one_dimensional::x_axis()
    {
        return f_x_axis;
    }
    const plot::abscissa& plot::plot_one_dimensional::x_axis() const
    {
        return f_x_axis;
    }

    plot::ordinate& plot::plot_one_dimensional::y_values()
    {
        return f_y_values;
    }
    const plot::ordinate& plot::plot_one_dimensional::y_values() const
    {
        return f_y_values;
    }

    plot::plot_two_dimensional::plot_two_dimensional( const string_t& p_key, const string_t& p_name, const string_t& p_title, const abscissa& p_x_axis, const abscissa& p_y_axis, const ordinate& p_z_values ) :
                f_key( p_key ),
                f_name( p_name ),
                f_title( p_title ),
                f_x_axis( p_x_axis ),
                f_y_axis( p_y_axis ),
                f_z_values( p_z_values )
    {
    }
    plot::plot_two_dimensional::~plot_two_dimensional()
    {
    }

    string& plot::plot_two_dimensional::key()
    {
        return f_key;
    }
    const string& plot::plot_two_dimensional::key() const
    {
        return f_key;
    }

    string& plot::plot_two_dimensional::name()
    {
        return f_name;
    }
    const string& plot::plot_two_dimensional::name() const
    {
        return f_name;
    }

    string& plot::plot_two_dimensional::title()
    {
        return f_title;
    }
    const string& plot::plot_two_dimensional::title() const
    {
        return f_title;
    }

    plot::abscissa& plot::plot_two_dimensional::x_axis()
    {
        return f_x_axis;
    }
    const plot::abscissa& plot::plot_two_dimensional::x_axis() const
    {
        return f_x_axis;
    }

    plot::abscissa& plot::plot_two_dimensional::y_axis()
    {
        return f_y_axis;
    }
    const plot::abscissa& plot::plot_two_dimensional::y_axis() const
    {
        return f_y_axis;
    }

    plot::ordinate& plot::plot_two_dimensional::z_values()
    {
        return f_z_values;
    }
    const plot::ordinate& plot::plot_two_dimensional::z_values() const
    {
        return f_z_values;
    }

    plot::graph_two_dimensional::graph_two_dimensional( const string_t& p_key, const string_t& p_name, const string_t& p_title, const abscissa& p_x_values, const abscissa& p_y_values ) :
                f_key( p_key ),
                f_name( p_name ),
                f_title( p_title ),
                f_x_values( p_x_values ),
                f_y_values( p_y_values )
    {
    }
    plot::graph_two_dimensional::~graph_two_dimensional()
    {
    }

    string& plot::graph_two_dimensional::key()
    {
        return f_key;
    }
    const string& plot::graph_two_dimensional::key() const
    {
        return f_key;
    }

    string& plot::graph_two_dimensional::name()
    {
        return f_name;
    }
    const string& plot::graph_two_dimensional::name() const
    {
        return f_name;
    }

    string& plot::graph_two_dimensional::title()
    {
        return f_title;
    }
    const string& plot::graph_two_dimensional::title() const
    {
        return f_title;
    }

    plot::abscissa& plot::graph_two_dimensional::x_values()
    {
        return f_x_values;
    }
    const plot::abscissa& plot::graph_two_dimensional::x_values() const
    {
        return f_x_values;
    }

    plot::abscissa& plot::graph_two_dimensional::y_values()
    {
        return f_y_values;
    }
    const plot::abscissa& plot::graph_two_dimensional::y_values() const
    {
        return f_y_values;
    }

    plot::plot() :
                f_mutex(),
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
    void plot::create_plot_one_dimensional( const string& p_key, const string& p_name, const string& p_title, const abscissa& p_x, const ordinate& p_y )
    {
        f_mutex.lock();

        msg_normal( msg, "creating one dimensional plot <" << p_name << "> on canvas <" << p_key << ">" << eom );

        f_plot1s.push( plot_one_dimensional( p_key, p_name, p_title, p_x, p_y ) );

        f_mutex.unlock();

        return;
    }
    void plot::create_plot_two_dimensional( const string& p_key, const string& p_name, const string& p_title, const abscissa& p_x, const abscissa& p_y, const ordinate& p_z )
    {
        f_mutex.lock();

        msg_normal( msg, "creating two dimensional plot <" << p_name << "> on canvas <" << p_key << ">" << eom );

        f_plot2s.push( plot_two_dimensional( p_key, p_name, p_title, p_x, p_y, p_z ) );

        f_mutex.unlock();

        return;
    }
    void plot::create_graph_two_dimensional( const string& p_key, const string& p_name, const string& p_title, const abscissa& p_x, const abscissa& p_y )
    {
        f_mutex.lock();

        msg_normal( msg, "creating two dimensional graph <" << p_name << "> on canvas <" << p_key << ">" << eom );

        f_graph2s.push( graph_two_dimensional( p_key, p_name, p_title, p_x, p_y ) );

        f_mutex.unlock();

        return;
    }
    void plot::finalize()
    {
        f_mutex.lock();

        if( --f_count == 0 )
        {
            while( f_plot1s.empty() == false )
            {
                draw_plot_one_dimensional( f_plot1s.top() );
                f_plot1s.pop();
            }

            while( f_plot2s.empty() == false )
            {
                draw_plot_two_dimensional( f_plot2s.top() );
                f_plot2s.pop();
            }

            while( f_graph2s.empty() == false )
            {
                draw_graph_two_dimensional( f_graph2s.top() );
                f_graph2s.pop();
            }

            if( f_plots.size() != 0 )
            {
                f_application->Run( kTRUE );
            }

            for( plot_it t_it = f_plots.begin(); t_it != f_plots.end(); ++t_it )
            {
                msg_normal( msg, "deleting canvas <" << t_it->first << ">" << eom );
                delete t_it->second.first;
            }
            for( th1_it t_it = f_th1s.begin(); t_it != f_th1s.end(); ++t_it )
            {
                msg_normal( msg, "deleting one dimensional plot <" << t_it->first << ">" << eom );
                delete t_it->second;
            }
            for( th2_it t_it = f_th2s.begin(); t_it != f_th2s.end(); ++t_it )
            {
                msg_normal( msg, "deleting two dimensional plot <" << t_it->first << ">" << eom );
                delete t_it->second;
            }
            for( graph_it t_it = f_graphs.begin(); t_it != f_graphs.end(); ++t_it )
            {
                msg_normal( msg, "deleting two dimensional graph <" << t_it->first << ">" << eom );
                delete t_it->second;
            }

            delete f_application;
        }

        f_mutex.unlock();

        return;
    }

    void plot::draw_plot_one_dimensional( const plot_one_dimensional& p_plot )
    {
        if( p_plot.x_axis().values().size() != p_plot.y_values().values().size() )
        {
            throw error() << "plot one dimensional was given x values with size <" << p_plot.x_axis().values().size() << "> and y values with size <" << p_plot.y_values().values().size() << ">";
        }

        msg_normal( msg, "drawing one dimensional plot <" << p_plot.name() << "> on canvas <" << p_plot.key() << ">" << eom );

        real_t t_x_increment = (p_plot.x_axis().high() - p_plot.x_axis().low()) / p_plot.x_axis().count();

        TH1D* t_histogram;
        th1_it t_hist_it = f_th1s.find( p_plot.name() );
        if( t_hist_it != f_th1s.end() )
        {
            t_histogram = t_hist_it->second;
        }
        else
        {
            t_histogram = new TH1D( p_plot.name().c_str(), p_plot.name().c_str(), p_plot.x_axis().count(), p_plot.x_axis().low() - .5 * t_x_increment, p_plot.x_axis().high() + .5 * t_x_increment );
            t_histogram->SetDirectory( NULL );
            f_th1s.insert( th1_entry( p_plot.name(), t_histogram ) );
        }

        for( index_t t_index = 0; t_index < p_plot.x_axis().values().size(); t_index++ )
        {
            if( p_plot.y_values().values().at( t_index ) != p_plot.y_values().values().at( t_index ) )
            {
                continue;
            }
            t_histogram->SetBinContent( t_histogram->FindFixBin( p_plot.x_axis().values().at( t_index ) ), p_plot.y_values().values().at( t_index ) );
        }
        real_t t_y_low = t_histogram->GetMinimum();
        real_t t_y_high = t_histogram->GetMaximum();
        real_t t_y_increment = (t_y_high - t_y_low) / 8.;

        TCanvas* t_canvas;
        TH1* t_axes;
        plot_it t_plot_it = f_plots.find( p_plot.key() );
        if( t_plot_it != f_plots.end() )
        {
            t_canvas = t_plot_it->second.first;
            t_canvas->cd( 0 );
            t_axes = t_plot_it->second.second;
            t_axes->SetTitle( p_plot.title().c_str() );
            t_axes->GetXaxis()->SetTitle( p_plot.x_axis().title().c_str() );
            t_axes->GetXaxis()->SetTitleOffset( 1.2 );
            t_axes->GetYaxis()->SetTitle( p_plot.y_values().title().c_str() );
            t_axes->GetYaxis()->SetTitleOffset( 1.2 );
            t_histogram->SetLineColor( kRed );
            t_histogram->SetLineWidth( 2 );
            t_histogram->SetStats( false );
            t_histogram->Draw( "L SAME" );
        }
        else
        {
            t_canvas = new TCanvas( p_plot.key().c_str() );
            t_canvas->SetWindowSize( 1024, 768 );
            t_canvas->SetFillColor( kWhite );
            t_canvas->SetLeftMargin( .12 );
            t_canvas->SetRightMargin( .08 );
            t_canvas->SetBottomMargin( .12 );
            t_canvas->cd( 0 );
            t_axes = t_canvas->DrawFrame( p_plot.x_axis().low() - .5 * t_x_increment, t_y_low - .5 * t_y_increment, p_plot.x_axis().high() + .5 * t_x_increment, t_y_high + .5 * t_y_increment );
            t_axes->SetStats( kFALSE );
            t_axes->SetTitle( p_plot.title().c_str() );
            t_axes->GetXaxis()->SetTitle( p_plot.x_axis().title().c_str() );
            t_axes->GetXaxis()->SetTitleOffset( 1.2 );
            t_axes->GetYaxis()->SetTitle( p_plot.y_values().title().c_str() );
            t_axes->GetYaxis()->SetTitleOffset( 1.2 );
            t_histogram->SetLineColor( kRed );
            t_histogram->SetLineWidth( 2 );
            t_histogram->SetStats( false );
            t_histogram->Draw( "L SAME" );
            f_plots.insert( plot_entry( p_plot.key(), plot_pair( t_canvas, t_axes ) ) );
        }

        return;
    }
    void plot::draw_plot_two_dimensional( const plot_two_dimensional& p_plot )
    {
        if( p_plot.z_values().values().size() != p_plot.x_axis().values().size() )
        {
            throw error() << "time frequency plotter was given z values with size <" << p_plot.z_values().values().size() << "> and x values with size <" << p_plot.x_axis().values().size() << ">";
        }

        if( p_plot.z_values().values().size() != p_plot.y_axis().values().size() )
        {
            throw error() << "time frequency plotter was given z values with size <" << p_plot.z_values().values().size() << "> and y values with size <" << p_plot.y_axis().values().size() << ">";
        }

        msg_normal( msg, "drawing two dimensional plot <" << p_plot.name() << "> on canvas <" << p_plot.key() << ">" << eom );

        real_t t_x_increment = (p_plot.x_axis().high() - p_plot.x_axis().low()) / p_plot.x_axis().count();
        real_t t_y_increment = (p_plot.y_axis().high() - p_plot.y_axis().low()) / p_plot.y_axis().count();

        TH2D* t_histogram;
        th2_it t_hist_it = f_th2s.find( p_plot.name() );
        if( t_hist_it != f_th2s.end() )
        {
            t_histogram = t_hist_it->second;
        }
        else
        {
            t_histogram = new TH2D( p_plot.name().c_str(), p_plot.name().c_str(), p_plot.x_axis().count(), p_plot.x_axis().low() - .5 * t_x_increment, p_plot.x_axis().high() + .5 * t_x_increment, p_plot.y_axis().count(), p_plot.y_axis().low() - .5 * t_y_increment, p_plot.y_axis().high() + .5 * t_y_increment );
            t_histogram->SetDirectory( NULL );
            f_th2s.insert( th2_entry( p_plot.name(), t_histogram ) );
        }

        for( index_t t_index = 0; t_index < p_plot.z_values().values().size(); t_index++ )
        {
            if( p_plot.z_values().values().at( t_index ) != p_plot.z_values().values().at( t_index ) )
            {
                continue;
            }
            t_histogram->SetBinContent( t_histogram->FindFixBin( p_plot.x_axis().values().at( t_index ), p_plot.y_axis().values().at( t_index ) ), p_plot.z_values().values().at( t_index ) );
        }

        TCanvas* t_canvas;
        TH1* t_axes;
        plot_it t_plot_it = f_plots.find( p_plot.key() );
        if( t_plot_it != f_plots.end() )
        {
            t_canvas = t_plot_it->second.first;
            t_canvas->cd( 0 );
            t_axes = t_plot_it->second.second;
            t_axes->SetTitle( p_plot.title().c_str() );
            t_axes->GetXaxis()->SetTitle( p_plot.x_axis().title().c_str() );
            t_axes->GetXaxis()->SetTitleOffset( 1.2 );
            t_axes->GetYaxis()->SetTitle( p_plot.y_axis().title().c_str() );
            t_axes->GetYaxis()->SetTitleOffset( 1.2 );
            t_histogram->GetZaxis()->SetTitle( p_plot.z_values().title().c_str() );
            t_histogram->GetZaxis()->SetTitleOffset( 1.2 );
            t_histogram->SetStats( kFALSE );
            t_histogram->Draw( "COL SAME" );
        }
        else
        {
            t_canvas = new TCanvas( p_plot.key().c_str() );
            t_canvas->SetWindowSize( 1024, 768 );
            t_canvas->SetFillColor( kWhite );
            t_canvas->SetLeftMargin( .10 );
            t_canvas->SetRightMargin( .15 );
            t_canvas->SetBottomMargin( .12 );
            t_canvas->cd( 0 );
            t_axes = t_canvas->DrawFrame( p_plot.x_axis().low() - .5 * t_x_increment, p_plot.y_axis().low() - .5 * t_y_increment, p_plot.x_axis().high() + .5 * t_x_increment, p_plot.y_axis().high() + .5 * t_y_increment );
            t_axes->SetTitle( p_plot.title().c_str() );
            t_axes->GetXaxis()->SetTitle( p_plot.x_axis().title().c_str() );
            t_axes->GetXaxis()->SetTitleOffset( 1.2 );
            t_axes->GetYaxis()->SetTitle( p_plot.y_axis().title().c_str() );
            t_axes->GetYaxis()->SetTitleOffset( 1.2 );
            t_histogram->GetZaxis()->SetTitle( p_plot.z_values().title().c_str() );
            t_histogram->GetZaxis()->SetTitleOffset( 1.2 );
            t_histogram->SetStats( kFALSE );
            t_histogram->Draw( "COLZ SAME" );
            f_plots.insert( plot_entry( p_plot.key(), plot_pair( t_canvas, t_axes ) ) );
        }

        return;
    }
    void plot::draw_graph_two_dimensional( const graph_two_dimensional& p_graph )
    {
        if( p_graph.y_values().values().size() != p_graph.x_values().values().size() )
        {
            throw error() << "time frequency plotter was given z values with size <" << p_graph.y_values().values().size() << "> and x values with size <" << p_graph.x_values().values().size() << ">";
        }

        msg_normal( msg, "drawing two dimensional graph <" << p_graph.name() << "> on canvas <" << p_graph.key() << ">" << eom );

        real_t t_x_increment = (p_graph.x_values().high() - p_graph.x_values().low()) / p_graph.x_values().count();
        real_t t_y_increment = (p_graph.y_values().high() - p_graph.y_values().low()) / p_graph.y_values().count();

        TGraph* t_graph = new TGraph();
        f_graphs.insert( graph_entry( p_graph.name(), t_graph ) );

        for( index_t t_index = 0; t_index < p_graph.y_values().values().size(); t_index++ )
        {
            if( p_graph.y_values().values().at( t_index ) != p_graph.y_values().values().at( t_index ) )
            {
                continue;
            }
            t_graph->SetPoint( t_graph->GetN(), p_graph.x_values().values().at( t_index ), p_graph.y_values().values().at( t_index ) );
        }

        TCanvas* t_canvas;
        TH1* t_axes;
        plot_it t_plot_it = f_plots.find( p_graph.key() );
        if( t_plot_it != f_plots.end() )
        {
            t_canvas = t_plot_it->second.first;
            t_canvas->cd( 0 );
            t_axes = t_plot_it->second.second;
            t_axes->SetTitle( p_graph.title().c_str() );
            t_axes->GetXaxis()->SetTitle( p_graph.x_values().title().c_str() );
            t_axes->GetXaxis()->SetTitleOffset( 1.2 );
            t_axes->GetYaxis()->SetTitle( p_graph.y_values().title().c_str() );
            t_axes->GetYaxis()->SetTitleOffset( 1.2 );
            t_graph->SetLineWidth( 2 );
            t_graph->SetLineColor( kRed );
            t_graph->Draw( "L" );
        }
        else
        {
            t_canvas = new TCanvas( p_graph.key().c_str() );
            t_canvas->SetWindowSize( 1024, 768 );
            t_canvas->SetFillColor( kWhite );
            t_canvas->SetLeftMargin( .10 );
            t_canvas->SetRightMargin( .15 );
            t_canvas->SetBottomMargin( .12 );
            t_canvas->cd( 0 );
            t_axes = t_canvas->DrawFrame( p_graph.x_values().low() - .5 * t_x_increment, p_graph.y_values().low() - .5 * t_y_increment, p_graph.x_values().high() + .5 * t_x_increment, p_graph.y_values().high() + .5 * t_y_increment );
            t_axes->SetTitle( p_graph.title().c_str() );
            t_axes->GetXaxis()->SetTitle( p_graph.x_values().title().c_str() );
            t_axes->GetXaxis()->SetTitleOffset( 1.2 );
            t_axes->GetYaxis()->SetTitle( p_graph.y_values().title().c_str() );
            t_axes->GetYaxis()->SetTitleOffset( 1.2 );
            t_graph->SetLineWidth( 2 );
            t_graph->SetLineColor( kRed );
            t_graph->Draw( "L" );
            f_plots.insert( plot_entry( p_graph.key(), plot_pair( t_canvas, t_axes ) ) );
        }

        return;
    }

}
