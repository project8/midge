#ifndef _midge_line_plot_consumer_hh_
#define _midge_line_plot_consumer_hh_

#include "consumer.hh"
#include "line_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class line_plot_consumer :
        public _consumer< line_plot_consumer, typelist_1( line_data )>
    {
        public:
            line_plot_consumer();
            virtual ~line_plot_consumer();

        public:
            accessible( bool, plot_overlay )
            accessible( string, plot_field )
            accessible( string, plot_key )
            accessible( string, plot_name )
            accessible( string, chart_title )
            accessible( string, x_title )
            accessible( string, y_title )
            accessible( string, z_title )

        public:
            void initialize();
            void execute();
            void finalize();
    };

}

#endif
