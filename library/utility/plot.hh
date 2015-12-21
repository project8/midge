#ifndef _midge_plot_hh_
#define _midge_plot_hh_

#include "TApplication.h"
#include "TStyle.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"

#include <stack>

#include "mutex.hh"
#include "singleton.hh"
#include "types.hh"
using std::stack;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <utility>
using std::pair;

namespace midge
{

    class plot :
        public singleton< plot >
    {
        public:
            friend class singleton< plot > ;

        public:
            typedef vector< real_t > values_t;
            typedef vector< real_t >::iterator value_it;
            typedef vector< real_t >::const_iterator value_cit;

        public:
            class ordinate
            {
                public:
                    ordinate();
                    ordinate( const count_t& p_size );
                    ~ordinate();

                    void operator()( const count_t& p_size );

                public:
                    string_t& title();
                    const string_t& title() const;

                    values_t& values();
                    const values_t& values() const;

                private:
                    string_t f_title;
                    values_t f_values;
            };

            class abscissa
            {
                public:
                    abscissa();
                    abscissa( const count_t& p_size );
                    ~abscissa();

                    void operator()( const count_t& p_size );

                public:
                    string_t& title();
                    const string_t& title() const;

                    count_t& count();
                    const count_t& count() const;

                    real_t& low();
                    const real_t& low() const;

                    real_t& high();
                    const real_t& high() const;

                    values_t& values();
                    const values_t& values() const;

                private:
                    string_t f_title;
                    count_t f_count;
                    real_t f_low;
                    real_t f_high;
                    values_t f_values;
            };

            class plot_one_dimensional
            {
                public:
                    plot_one_dimensional( const string_t& p_key, const string_t& p_name, const string_t& p_title, const abscissa& p_x_axis, const ordinate& p_y_values );
                    ~plot_one_dimensional();

                    string& key();
                    const string& key() const;

                    string& name();
                    const string& name() const;

                    string& title();
                    const string& title() const;

                    abscissa& x_axis();
                    const abscissa& x_axis() const;

                    ordinate& y_values();
                    const ordinate& y_values() const;

                private:
                    string f_key;
                    string f_name;
                    string f_title;
                    abscissa f_x_axis;
                    ordinate f_y_values;
            };

            class plot_two_dimensional
            {
                public:
                    plot_two_dimensional( const string_t& p_key, const string_t& p_name, const string_t& p_title, const abscissa& p_x_axis, const abscissa& p_y_axis, const ordinate& p_z_values );
                    ~plot_two_dimensional();

                    string& key();
                    const string& key() const;

                    string& name();
                    const string& name() const;

                    string& title();
                    const string& title() const;

                    abscissa& x_axis();
                    const abscissa& x_axis() const;

                    abscissa& y_axis();
                    const abscissa& y_axis() const;

                    ordinate& z_values();
                    const ordinate& z_values() const;

                private:
                    string f_key;
                    string f_name;
                    string f_title;
                    abscissa f_x_axis;
                    abscissa f_y_axis;
                    ordinate f_z_values;
            };

            class graph_two_dimensional
            {
                public:
                    graph_two_dimensional( const string_t& p_key, const string_t& p_name, const string_t& p_title, const abscissa& p_x_values, const abscissa& p_y_values );
                    ~graph_two_dimensional();

                    string& key();
                    const string& key() const;

                    string& name();
                    const string& name() const;

                    string& title();
                    const string& title() const;

                    abscissa& x_values();
                    const abscissa& x_values() const;

                    abscissa& y_values();
                    const abscissa& y_values() const;

                private:
                    string f_key;
                    string f_name;
                    string f_title;
                    abscissa f_x_values;
                    abscissa f_y_values;
            };

        private:
            plot();
            ~plot();

        public:
            void initialize();
            void create_plot_one_dimensional( const string_t& p_key, const string_t& p_name, const string_t& p_title, const abscissa& p_x_axis, const ordinate& p_y_values );
            void create_plot_two_dimensional( const string_t& p_key, const string_t& p_name, const string_t& p_title, const abscissa& p_x_axis, const abscissa& p_y_axis, const ordinate& p_z_values );
            void create_graph_two_dimensional( const string_t& p_key, const string_t& p_name, const string_t& p_title, const abscissa& p_x_axis, const abscissa& p_y_axis );
            void finalize();

        private:
            void draw_plot_one_dimensional( const plot_one_dimensional& p_plot );
            void draw_plot_two_dimensional( const plot_two_dimensional& p_plot );
            void draw_graph_two_dimensional( const graph_two_dimensional& p_graph );

            mutex f_mutex;
            count_t f_count;
            TApplication* f_application;

            typedef stack< plot_one_dimensional > plot1_stack;

            plot1_stack f_plot1s;

            typedef stack< plot_two_dimensional > plot2_stack;

            plot2_stack f_plot2s;

            typedef stack< graph_two_dimensional > graph2_stack;

            graph2_stack f_graph2s;

            typedef pair< TCanvas*, TH1* > plot_pair;
            typedef map< string_t, pair< TCanvas*, TH1* > > plot_map;
            typedef plot_map::iterator plot_it;
            typedef plot_map::const_iterator plot_cit;
            typedef plot_map::value_type plot_entry;

            plot_map f_plots;

            typedef map< string_t, TH1D* > th1_map;
            typedef th1_map::iterator th1_it;
            typedef th1_map::const_iterator th1_cit;
            typedef th1_map::value_type th1_entry;

            th1_map f_th1s;

            typedef map< string_t, TH2D* > th2_map;
            typedef th2_map::iterator th2_it;
            typedef th2_map::const_iterator th2_cit;
            typedef th2_map::value_type th2_entry;

            th2_map f_th2s;

            typedef map< string_t, TGraph* > graph_map;
            typedef graph_map::iterator graph_it;
            typedef graph_map::const_iterator graph_cit;
            typedef graph_map::value_type graph_entry;

            graph_map f_graphs;
    };

}

#endif
