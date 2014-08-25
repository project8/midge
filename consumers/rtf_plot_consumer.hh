#include "consumer.hh"
#include "rtf_data.hh"
#include "typelist.hh"
#include "macros.hh"

namespace midge
{

    class rtf_plot_consumer :
        public _consumer< rtf_plot_consumer, typelist_1( rtf_data )>
    {
        public:
            rtf_plot_consumer();
            virtual ~rtf_plot_consumer();

        public:
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
