#include "consumer.hh"
#include "rt_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class rt_plot_consumer :
        public _consumer< rt_plot_consumer, typelist_1( rt_data )>
    {
        public:
            rt_plot_consumer();
            virtual ~rt_plot_consumer();

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
