#ifndef _midge_plot_hh_
#define _midge_plot_hh_

#include "singleton.hh"
#include "types.hh"

#include "TApplication.h"
#include "TStyle.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"

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
                    ordinate( const count_t& t_size );
                    ~ordinate();

                    void operator()( const count_t& t_size );

                public:
                    string_t& title();
                    const string_t& title() const;

                    values_t& values();
                    const values_t& values() const;

                private:
                    ordinate( const ordinate& );

                    string_t f_title;
                    values_t f_values;
            };

            class abscissa
            {
                public:
                    abscissa();
                    abscissa( const count_t& t_size );
                    ~abscissa();

                    void operator()( const count_t& t_size );

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
                    abscissa( const abscissa& );

                    string_t f_title;
                    count_t f_count;
                    real_t f_low;
                    real_t f_high;
                    values_t f_values;
            };

        private:
            plot();
            ~plot();

        public:
            void initialize();
            void plot_one_dimensional
            (
                const string_t& p_key,
                const string_t& p_name,
                const string_t& p_title,
                const abscissa& p_x_axis,
                const ordinate& p_y_values
            );
            void plot_two_dimensional
            (
                const string_t& p_key,
                const string_t& p_name,
                const string_t& p_title,
                const abscissa& p_x_axis,
                const abscissa& p_y_axis,
                const ordinate& p_z_values
            );
            void graph_two_dimensional
            (
                const string_t& p_key,
                const string_t& p_name,
                const string_t& p_title,
                const abscissa& p_x_axis,
                const abscissa& p_y_axis
            );
            void finalize();

        private:
            count_t f_count;
            TApplication* f_application;

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
