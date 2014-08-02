#ifndef _midge_plot_hh_
#define _midge_plot_hh_

#include "singleton.hh"
#include "node.hh"

#include "TApplication.h"
#include "TCanvas.h"
#include "TAxis.h"
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

            typedef vector< real_t > data;
            typedef vector< real_t >::iterator data_it;
            typedef vector< real_t >::const_iterator data_cit;

        private:
            plot();
            ~plot();

        public:
            void initialize();
            void plot_time_series( const string& p_label, const string& p_title, const string& p_time_axis, const string& p_value_axis, const vector< real_t >& p_time_coordinates, const vector< real_t >& p_value_coordinates );
            void plot_frequency_series( const string& p_label, const string& p_title, const string& p_frequency_axis, const string& p_value_axis, const vector< real_t >& p_frequency_coordinates, const vector< real_t >& p_value_coordinates );
            void plot_time_frequency_series( const string& p_label, const string& p_title, const string& p_time_axis, const string& p_frequency_axis, const string& p_value_axis, const vector< real_t >& p_time_coordinates, const vector< real_t >& p_frequency_coordinates, const vector< real_t >& p_value_coordinates );
            void finalize();

        private:
            class plotter
            {
                public:
                    plotter();
                    virtual ~plotter();
            };

            class time_plotter :
                public plotter
            {
                public:
                    time_plotter( const string& p_label, const string& p_title, const string& p_time_axis, const string& p_value_axis, const vector< real_t >& p_time_coordinates, const vector< real_t >& p_value_coordinates );
                    virtual ~time_plotter();

                private:
                    TCanvas* f_canvas;
                    TH1D* f_histogram;
            };

            class frequency_plotter :
                public plotter
            {
                public:
                    frequency_plotter( const string& p_label, const string& p_title, const string& p_frequency_axis, const string& p_value_axis, const vector< real_t >& p_frequency_coordinates, const vector< real_t >& p_value_coordinates );
                    virtual ~frequency_plotter();

                private:
                    TCanvas* f_canvas;
                    TH1D* f_histogram;
            };

            class time_frequency_plotter :
                public plotter
            {
                public:
                    time_frequency_plotter( const string& p_label, const string& p_title, const string& p_time_axis, const string& p_frequency_axis, const string& p_value_axis, const vector< real_t >& p_time_coordinates, const vector< real_t >& p_frequency_coordinates, const vector< real_t >& p_value_coordinates );
                    virtual ~time_frequency_plotter();

                private:
                    TCanvas* f_canvas;
                    TH2D* f_histogram;
            };

            count_t f_count;
            vector< plotter* > f_plotters;

            TApplication* f_application;
    };

}

#endif
