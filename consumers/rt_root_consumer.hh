#ifndef _midge_rt_root_consumer_hh_
#define _midge_rt_root_consumer_hh_

#include "consumer.hh"
#include "rt_data.hh"
#include "typelist.hh"
#include "macros.hh"

#include "TFile.h"
#include "TTree.h"

namespace midge
{

    class rt_root_consumer :
        public _consumer< rt_root_consumer, typelist_1( rt_data )>
    {
        public:
            rt_root_consumer();
            virtual ~rt_root_consumer();

        public:
            accessible( string, file )
            accessible( bool_t, plot )
            accessible( string, plot_key )
            accessible( string, plot_name )
            accessible( string, chart_title )
            accessible( string, axis_title )

        private:
            TFile* f_stream;
            TTree* f_tree;
            real_t f_time;
            real_t f_value;

            count_t f_size;
            real_t f_interval;
            const real_t* f_in;
            count_t f_index;

        protected:
            bool start_consumer();
            bool execute_consumer();
            bool stop_consumer();
    };

}

#endif
