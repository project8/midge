#include "consumer.hh"
#include "rf_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class rf_plot_consumer :
        public _consumer< rf_plot_consumer, typelist_1( rf_data )>
    {
        public:
            rf_plot_consumer();
            virtual ~rf_plot_consumer();

        public:
            accessible( string, plot_key )
            accessible( string, plot_name )
            accessible( string, chart_title )
            accessible( string, x_title )
            accessible( string, y_title )

        public:
            void initialize();
            void execute();
            void finalize();
    };

}
