#ifndef _midge_event_plot_consumer_hh_
#define _midge_event_plot_consumer_hh_

#include "consumer.hh"
#include "event_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class event_plot_consumer :
        public _consumer< event_plot_consumer, typelist_1( event_data )>
    {
        public:
            event_plot_consumer();
            virtual ~event_plot_consumer();

        public:
            accessible( bool, plot_undermap )
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
