#include "consumer.hh"
#include "point_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class point_plot_consumer :
        public _consumer< point_plot_consumer, typelist_1( point_data )>
    {
        public:
            point_plot_consumer();
            virtual ~point_plot_consumer();

        public:
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
