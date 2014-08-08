#ifndef _midge_plot_hh_
#define _midge_plot_hh_

#include "singleton.hh"
#include "node.hh"

#include "TApplication.h"
#include "TStyle.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"

#include <set>
using std::set;

#include <vector>
using std::vector;

namespace midge
{


    class plot :
        public singleton< plot >
    {
        public:
            friend class singleton< plot > ;

        public:
            typedef vector< real_t > data;
            typedef vector< real_t >::iterator data_it;
            typedef vector< real_t >::const_iterator data_cit;

        public:
            class ordinate
            {
                public:
                    ordinate();
                    ordinate( const count_t& t_size );
                    ~ordinate();

                    void operator()( const count_t& t_size );

                public:
                    string& title();
                    const string& title() const;

                    data& values();
                    const data& values() const;

                private:
                    ordinate( const ordinate& );

                    string f_title;
                    data f_values;
            };

            class abscissa
            {
                public:
                    abscissa();
                    abscissa( const count_t& t_size );
                    ~abscissa();

                    void operator()( const count_t& t_size );

                public:
                    string& title();
                    const string& title() const;

                    count_t& count();
                    const count_t& count() const;

                    real_t& low();
                    const real_t& low() const;

                    real_t& high();
                    const real_t& high() const;

                    data& values();
                    const data& values() const;

                private:
                    abscissa( const abscissa& );

                    string f_title;
                    count_t f_count;
                    real_t f_low;
                    real_t f_high;
                    data f_values;
            };

        private:
            plot();
            ~plot();

        public:
            void initialize();
            void plot_one_dimensional
            (
                const string& p_key,
                const string& p_name,
                const string& p_title,
                const abscissa& p_x_axis,
                const ordinate& p_y_values
            );
            void plot_two_dimensional
            (
                const string& p_key,
                const string& p_name,
                const string& p_title,
                const abscissa& p_x_axis,
                const abscissa& p_y_axis,
                const ordinate& p_z_values
            );
            void graph_two_dimensional
            (
                const string& p_key,
                const string& p_name,
                const string& p_title,
                const abscissa& p_x_axis,
                const abscissa& p_y_axis
            );
            void finalize();

        private:
            count_t f_count;
            TApplication* f_application;

            typedef map< string, TCanvas* > canvas_map;
            typedef canvas_map::iterator canvas_it;
            typedef canvas_map::const_iterator canvas_cit;
            typedef canvas_map::value_type canvas_entry;

            canvas_map f_canvases;

            typedef vector< TH1D* > th1_vector;
            typedef th1_vector::iterator th1_it;
            typedef th1_vector::const_iterator th1_cit;

            th1_vector f_th1s;

            typedef vector< TH2D* > th2_vector;
            typedef th2_vector::iterator th2_it;
            typedef th2_vector::const_iterator th2_cit;

            th2_vector f_th2s;

            typedef vector< TGraph* > graph_vector;
            typedef graph_vector::iterator graph_it;
            typedef graph_vector::const_iterator graph_cit;

            graph_vector f_graphs;
    };

}

#endif
