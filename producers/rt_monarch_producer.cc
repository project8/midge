#include "rt_monarch_producer.hh"

#include "Monarch.hpp"

#include <cmath>

namespace midge
{

    rt_monarch_producer::rt_monarch_producer() :
            f_file( "" ),
            f_begin_sec( 0. ),
            f_end_sec( 1. ),
            f_size( 0 ),
            f_stride( 0 ),
            f_length( 10 )
    {
    }
    rt_monarch_producer::~rt_monarch_producer()
    {
    }

    void rt_monarch_producer::initialize()
    {
        out_buffer< 0 >().initialize( f_length );
        return;
    }

    void rt_monarch_producer::execute()
    {
        count_t t_index;

        rt_data t_data;
        real_t* t_current_raw;
        real_t* t_previous_raw;

        const monarch::Monarch* t_monarch = monarch::Monarch::OpenForReading( f_file );

        t_monarch->ReadHeader();
        const monarch::MonarchHeader* t_header = t_monarch->GetHeader();

        t_monarch->SetInterface( monarch::sInterfaceSeparate );
        const monarch::MonarchRecord< monarch::DataType >* t_record = t_monarch->GetRecordSeparateOne();

        real_t t_time_interval = 1.e-6 / t_header->GetAcquisitionRate();

        real_t t_datum;
        real_t t_voltage_minimum = t_header->GetVoltageMin();
        real_t t_voltage_range = t_header->GetVoltageRange();
        real_t t_voltage_levels = 1 << t_header->GetBitDepth();
        real_t t_voltage_inverse_levels = 1. / t_voltage_levels;

        count_t t_begin = (count_t) (round( f_begin_sec / t_time_interval ));
        count_t t_end = (count_t) (round( f_end_sec / t_time_interval ));
        count_t t_record_length = t_header->GetRecordSize();
        count_t t_first_unwritten_index;
        count_t t_first_requested_index;
        count_t t_record_index;

        out_stream< 0 >() >> t_data;
        t_data.set_size( f_size );
        t_data.set_time_interval( t_time_interval );
        t_data.set_time_index( t_begin );
        out_stream< 0 >().command( stream::s_start );
        out_stream< 0 >() << t_data;

        t_first_unwritten_index = 0;
        t_first_requested_index = t_begin;
        t_record_index = t_record_length;
        while( true )
        {
            if( (out_stream< 0 >().command() == stream::s_stop) || (t_first_unwritten_index >= t_end) )
            {
                out_stream< 0 >() >> t_data;
                out_stream< 0 >().command( stream::s_stop );
                out_stream< 0 >() << t_data;

                out_stream< 0 >() >> t_data;
                out_stream< 0 >().command( stream::s_exit );
                out_stream< 0 >() << t_data;

                return;
            }

            out_stream< 0 >() >> t_data;

            t_data.set_size( f_size );
            t_data.set_time_interval( t_time_interval );
            t_data.set_time_index( t_first_requested_index );
            t_current_raw = t_data.raw();

            if( t_first_unwritten_index > t_first_requested_index )
            {
                for( t_index = t_first_requested_index; t_index < t_first_unwritten_index; t_index++ )
                {
                    t_current_raw[ t_index - t_first_requested_index ] = t_previous_raw[ t_index - t_first_unwritten_index + f_size ];
                }
                for( t_index = t_first_unwritten_index; t_index < t_first_requested_index + f_size; t_index++ )
                {
                    while( t_record_index >= t_record_length )
                    {
                        if( t_monarch->ReadRecord() == false )
                        {
                            t_monarch->Close();
                            delete t_monarch;

                            out_stream< 0 >() >> t_data;
                            out_stream< 0 >().command( stream::s_stop );
                            out_stream< 0 >() << t_data;

                            out_stream< 0 >() >> t_data;
                            out_stream< 0 >().command( stream::s_exit );
                            out_stream< 0 >() << t_data;

                            return;
                        }
                        t_record_index -= t_record_length;
                    }

                    t_datum = t_voltage_minimum + t_voltage_range * (real_t) (t_record->fData[ t_record_index ]) * t_voltage_inverse_levels;
                    t_current_raw[ t_index - t_first_requested_index ] = t_datum;

                    t_record_index++;
                }
            }
            else
            {
                t_record_index += t_first_requested_index - t_first_unwritten_index;

                for( t_index = t_first_requested_index; t_index < t_first_requested_index + f_size; t_index++ )
                {
                    while( t_record_index >= t_record_length )
                    {
                        if( t_monarch->ReadRecord() == false )
                        {
                            t_monarch->Close();
                            delete t_monarch;

                            out_stream< 0 >() >> t_data;
                            out_stream< 0 >().command( stream::s_stop );
                            out_stream< 0 >() << t_data;

                            out_stream< 0 >() >> t_data;
                            out_stream< 0 >().command( stream::s_exit );
                            out_stream< 0 >() << t_data;

                            return;
                        }
                        t_record_index -= t_record_length;
                    }

                    t_datum = t_voltage_minimum + t_voltage_range * (real_t) (t_record->fData[ t_record_index ]) * t_voltage_inverse_levels;
                    t_current_raw[ t_index - t_first_requested_index ] = t_datum;

                    t_record_index++;
                }
            }

            t_first_unwritten_index = t_first_requested_index + f_size;
            t_first_requested_index = t_first_requested_index + f_stride;
            t_previous_raw = t_current_raw;

            out_stream< 0 >().command( stream::s_run );
            out_stream< 0 >() << t_data;
        }

        return;
    }

    void rt_monarch_producer::finalize()
    {
        out_buffer< 0 >().finalize();
        return;
    }

}
