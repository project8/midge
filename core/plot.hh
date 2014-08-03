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

        public:
            typedef vector< real_t > data;
            typedef vector< real_t >::iterator data_it;
            typedef vector< real_t >::const_iterator data_cit;

        public:
            class ordinate
            {
                public:
                    ordinate( const count_t& t_size );
                    ~ordinate();

                public:
                    string& title();
                    const string& title() const;

                    data& values();
                    const data& values() const;

                private:
                    ordinate();
                    ordinate( const ordinate& );

                    string f_title;
                    data f_values;
            };

            class abscissa
            {
                public:
                    abscissa( const count_t& t_size );
                    ~abscissa();

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
                    abscissa();
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
                const string& p_label,
                const string& p_title,
                const abscissa& p_x_axis,
                const ordinate& p_y_values
            );
            void plot_two_dimensional
            (
                const string& p_label,
                const string& p_title,
                const abscissa& p_x_axis,
                const abscissa& p_y_axis,
                const ordinate& p_z_values
            );
            void finalize();

        private:
            count_t f_count;
            TApplication* f_application;
            vector< TCanvas* > f_canvases;
            vector< TH1D* > f_one_dimensional_plots;
            vector< TH2D* > f_two_dimensional_plots;
    };

}

#endif
