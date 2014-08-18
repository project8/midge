#include "rt_root_consumer.hh"

#include "TFile.h"
#include "TTree.h"

#include <cmath>

namespace midge
{

    rt_root_consumer::rt_root_consumer() :
            f_file( "" )
    {
    }
    rt_root_consumer::~rt_root_consumer()
    {
    }

    void rt_root_consumer::initialize()
    {
        return;
    }

    void rt_root_consumer::execute()
    {
        count_t t_index;

        state_t t_in_state;
        const rt_data* t_in_data;
        const real_t* t_in_raw;
        count_t t_in_size;
        real_t t_in_time_interval;
        count_t t_in_time_index;

        TFile* t_stream = NULL;
        TTree* t_tree = NULL;
        real_t t_time;
        real_t t_value;

        count_t t_first_unwritten_index;

        while( true )
        {
            in_stream< 0 >()++;
            t_in_state = in_stream< 0 >().state();
            t_in_data = in_stream< 0 >().data();

            if( t_in_state == stream::s_start )
            {
                t_in_size = t_in_data->get_size();
                t_in_time_interval = t_in_data->get_time_interval();
                t_in_time_index = t_in_data->get_time_index();

                t_stream = new TFile( f_file.c_str(), "RECREATE" );
                t_tree = new TTree( get_name().c_str(), get_name().c_str() );
                t_tree->SetDirectory( t_stream );
                t_tree->Branch( "time", &t_time, 64000, 99 );
                t_tree->Branch( "value", &t_value, 64000, 99 );

                t_first_unwritten_index = t_in_time_index;
                continue;
            }
            if( t_in_state == stream::s_run )
            {
                t_in_raw = t_in_data->raw();
                t_in_time_index = t_in_data->get_time_index();

                if( t_in_time_index < t_first_unwritten_index )
                {
                    for( t_index = t_first_unwritten_index; t_index < t_in_time_index + t_in_size; t_index++ )
                    {
                        t_time = t_index * t_in_time_interval;
                        t_value = t_in_raw[ t_index - t_first_unwritten_index ];
                        t_tree->Fill();
                    }
                }
                else
                {
                    for( t_index = t_first_unwritten_index; t_index < t_in_time_index; t_index++ )
                    {
                        t_time = t_index * t_in_time_interval;
                        t_value = 0.;
                        t_tree->Fill();
                    }
                    for( t_index = t_in_time_index; t_index < t_in_time_index + t_in_size; t_index++ )
                    {
                        t_time = t_index * t_in_time_interval;
                        t_value = t_in_raw[ t_index - t_first_unwritten_index ];
                        t_tree->Fill();
                    }
                }

                t_first_unwritten_index = t_in_time_index + t_in_size;

                continue;
            }
            if( t_in_state == stream::s_stop )
            {
                t_stream->cd();
                t_tree->Write();
                t_stream->Close();
                delete t_stream;

                continue;
            }
            if( t_in_state == stream::s_exit )
            {
                break;
            }
        }

        return;
    }

    void rt_root_consumer::finalize()
    {
        return;
    }

}
