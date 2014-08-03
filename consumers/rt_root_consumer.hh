#ifndef _midge_rt_root_consumer_hh_
#define _midge_rt_root_consumer_hh_

#include "consumer.hh"
#include "rt_data.hh"
#include "typelist.hh"

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
            void set_file( const string& p_filename );
            const string& get_file() const;

            void set_plot( const bool_t& p_plot );
            const bool_t& get_plot() const;

            void set_plot_title( const string& p_plot_title );
            const string& get_plot_title() const;

            void set_axis_title( const string& p_axis_title );
            const string& get_axis_title() const;

        private:
            string f_file;
            bool_t f_plot;
            string f_plot_title;
            string f_axis_title;

            TFile* f_stream;
            TTree* f_tree;
            real_t f_time;
            real_t f_value;

            count_t f_size;
            real_t f_interval;
            real_t* f_in;
            count_t f_index;

        protected:
            bool start_consumer();
            bool execute_consumer();
            bool stop_consumer();
    };

}

#endif
