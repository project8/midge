#include "event_spectrum_consumer.hh"

#include "TFile.h"
#include "TH1D.h"

#include <limits>
using std::numeric_limits;

namespace midge
{

    event_spectrum_consumer::event_spectrum_consumer() :
                f_file( "spectrum.root" ),
                f_frequency_mhz( 24200.0 ),
                f_field_gauss( 10000. ),
                f_start_kev( 32.0 ),
                f_stop_kev( 32.0 ),
                f_count( 100 )
    {
    }
    event_spectrum_consumer::~event_spectrum_consumer()
    {
    }

    void event_spectrum_consumer::initialize()
    {
        return;
    }

    void event_spectrum_consumer::execute()
    {
        static const real_t s_factor = 1.43041e3;
        static const real_t s_offset = 5.10999e2;

        real_t t_interval = (f_stop_kev - f_start_kev) / f_count;
        real_t t_start = f_start_kev - 0.5 * t_interval;
        real_t t_stop = f_stop_kev + 0.5 * t_interval;
        count_t t_count = f_count + 1;

        stringstream t_title;
        t_title << "Counts [number/second/" << t_interval * 1000 << " eV]";

        TFile* t_file = new TFile( f_file.c_str(), "UPDATE" );

        TH1D* t_multiplier = (TH1D*) (t_file->Get( "multiplier" ));
        if( t_multiplier == NULL )
        {
            t_multiplier = new TH1D( "multiplier", "multiplier", t_count, t_start, t_stop );
            t_multiplier->SetDirectory( t_file );
            t_multiplier->SetStats( kFALSE );
            t_multiplier->SetTitle( "Multiplier" );
            t_multiplier->GetXaxis()->SetTitle( "Energy [keV]" );
            t_multiplier->GetYaxis()->SetTitle( "Multiplier [none]");
        }
        TH1D* t_raw = (TH1D*) (t_file->Get( "raw" ));
        if( t_raw == NULL )
        {
            t_raw = new TH1D( "raw", "raw", t_count, t_start, t_stop );
            t_raw->SetDirectory( t_file );
            t_raw->SetStats( kFALSE );
            t_raw->SetTitle( "Raw Spectrum" );
            t_raw->GetXaxis()->SetTitle( "Energy [keV]" );
            t_raw->GetYaxis()->SetTitle( "Multiplier [none]");
        }
        TH1D* t_spectrum = (TH1D*) (t_file->Get( "spectrum" ));
        if( t_spectrum == NULL )
        {
            t_spectrum = new TH1D( "spectrum", "spectrum", t_count, t_start, t_stop );
            t_spectrum->SetDirectory( t_file );
            t_spectrum->SetStats( kFALSE );
            t_spectrum->SetTitle( "Project8 Krypton Energy Spectrum" );
            t_spectrum->GetXaxis()->SetTitle( "Energy [keV]" );
            t_spectrum->GetYaxis()->SetTitle( t_title.str().c_str() );
        }

        count_t t_bin;
        enum_t t_command;
        event_data* t_events;
        count_t t_size;
        real_t t_frequency_interval;
        count_t t_frequency_index;
        real_t t_time_interval;
        count_t t_time_index;

        real_t t_start_frequency_mhz;
        real_t t_stop_frequency_mhz;
        real_t t_event_frequency_mhz;

        real_t t_low_energy_kev;
        real_t t_high_energy_kev;
        real_t t_event_energy_kev;

        real_t t_start_time_sec = numeric_limits< real_t >::max();
        real_t t_stop_time_sec = numeric_limits< real_t >::min();
        real_t t_event_time_sec;

        while( true )
        {
            t_command = in_stream< 0 >().get();
            t_events = in_stream< 0 >().data();

            if( t_command == stream::s_start )
            {
                t_size = t_events->size();
                t_frequency_interval = t_events->frequency_interval();
                t_frequency_index = t_events->frequency_index();

                t_start_frequency_mhz = t_frequency_interval * t_frequency_index * 1.e-6 + f_frequency_mhz;
                t_stop_frequency_mhz = t_frequency_interval * (t_frequency_index + t_size - 1) * 1.e-6 + f_frequency_mhz;

                t_low_energy_kev = s_factor * f_field_gauss / t_stop_frequency_mhz - s_offset;
                t_high_energy_kev = s_factor * f_field_gauss / t_start_frequency_mhz - s_offset;

                for( t_bin = t_multiplier->FindFixBin( t_low_energy_kev ); t_bin <= t_multiplier->FindFixBin( t_high_energy_kev ); t_bin++ )
                {
                    t_multiplier->SetBinContent( t_bin, t_multiplier->GetBinContent( t_bin ) + 1. );
                }
            }
            if( t_command == stream::s_run )
            {
                t_size = t_events->size();
                t_frequency_interval = t_events->frequency_interval();
                t_frequency_index = t_events->frequency_index();
                t_time_interval = t_events->time_interval();
                t_time_index = t_events->time_index();

                t_event_time_sec = t_time_index * t_time_interval;

                for( vector< event >::const_iterator t_event_it = t_events->events().begin(); t_event_it != t_events->events().end(); t_event_it++ )
                {
                    t_event_frequency_mhz = t_event_it->frequency() * 1.e-6 + f_frequency_mhz;
                    t_event_energy_kev = s_factor * f_field_gauss / t_event_frequency_mhz - s_offset;
                    t_bin = t_raw->FindFixBin( t_event_energy_kev );
                    t_raw->SetBinContent( t_bin, t_raw->GetBinContent( t_bin ) + 1. );

                    msg_warning( msg, "got event of energy <" << t_event_energy_kev << "> at time <" << t_event_time_sec << ">" << eom );
                }

                if( t_start_time_sec > t_event_time_sec )
                {
                    t_start_time_sec = t_event_time_sec;
                }

                if( t_stop_time_sec < t_event_time_sec )
                {
                    t_stop_time_sec = t_event_time_sec;
                }

                continue;
            }
            if( t_command == stream::s_stop )
            {
                for( t_bin = t_spectrum->FindFixBin( t_low_energy_kev ); t_bin <= t_spectrum->FindFixBin( t_high_energy_kev ); t_bin++ )
                {
                    if( t_multiplier->GetBinContent( t_bin ) > 0.5 )
                    {
                        t_spectrum->SetBinContent( t_bin, t_raw->GetBinContent( t_bin ) / t_multiplier->GetBinContent( t_bin ) / (t_stop_time_sec - t_start_time_sec) );
                    }
                }

                continue;
            }
            if( t_command == stream::s_exit )
            {
                t_multiplier->Write( "", TObject::kOverwrite );
                t_raw->Write( "", TObject::kOverwrite );
                t_spectrum->Write( "", TObject::kOverwrite );
                t_file->Close();
                delete t_file;

                return;
            }
        }

        return;
    }

    void event_spectrum_consumer::finalize()
    {
        return;
    }

}
