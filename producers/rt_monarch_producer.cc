#include "rt_monarch_producer.hh"

#include <cmath>

namespace midge
{

    rt_monarch_producer::rt_monarch_producer() :
            f_file( "" ),
            f_offset( 0 ),
            f_stride( 0 ),
            f_size( 0 ),
            f_monarch( NULL ),
            f_header( NULL ),
            f_record( NULL ),
            f_length( 4194304 ),
            f_interval( 1.e-9 ),
            f_voltage_minimum( -.25 ),
            f_voltage_range( .5 ),
            f_voltage_inverse_range( 1. / .5 ),
            f_voltage_levels( 256. ),
            f_voltage_inverse_levels( 1. / 256. ),
            f_out( NULL ),
            f_index( 0 ),
            f_next( 0 ),
            f_samples( 0 )
    {
    }
    rt_monarch_producer::~rt_monarch_producer()
    {
    }

    void rt_monarch_producer::initialize_producer()
    {
        return;
    }

    bool rt_monarch_producer::start_producer()
    {
        f_monarch = monarch::Monarch::OpenForReading( f_file );

        f_monarch->ReadHeader();
        f_header = f_monarch->GetHeader();

        f_monarch->SetInterface( monarch::sInterfaceSeparate );
        f_record = f_monarch->GetRecordSeparateOne();

        f_length = f_header->GetRecordSize();
        f_interval = 1.e-6 / f_header->GetAcquisitionRate();
        f_voltage_minimum = f_header->GetVoltageMin();
        f_voltage_range = f_header->GetVoltageRange();
        f_voltage_inverse_range = 1. / f_voltage_range;
        f_voltage_levels = 1 << f_header->GetBitDepth();
        f_voltage_inverse_levels = 1. / f_voltage_levels;

        out< 0 >()->set_size( f_size );
        out< 0 >()->set_interval( f_interval );
        f_out = out< 0 >()->raw();
        f_index = 0;
        f_next = 0;
        f_samples = f_length;

        return true;
    }

    bool rt_monarch_producer::execute_producer()
    {
        count_t t_index;
        real_t t_datum;

        if( f_index != 0 )
        {
            f_next += f_stride;
        }
        else
        {
            f_next += f_offset;
        }

        if( f_index > f_next )
        {
            for( t_index = f_next; t_index < f_index; t_index++ )
            {
                f_out[ t_index - f_next ] = f_out[ t_index - f_index + f_size ];
            }

            for( t_index = f_index; t_index < f_next + f_size; t_index++ )
            {
                while( f_samples >= f_length )
                {
                    if( f_monarch->ReadRecord() == false )
                    {
                        return false;
                    }
                    f_samples -= f_length;
                }

                t_datum = f_voltage_minimum + f_voltage_range * (real_t) (f_record->fData[ f_samples ]) * f_voltage_inverse_levels;
                f_out[ t_index - f_next ] = t_datum;

                f_samples++;
            }
        }
        else
        {
            f_samples += f_next - f_index;

            for( t_index = f_next; t_index < f_next + f_size; t_index++ )
            {
                while( f_samples >= f_length )
                {
                    if( f_monarch->ReadRecord() == false )
                    {
                        return false;
                    }
                    f_samples -= f_length;
                }

                t_datum = f_voltage_minimum + f_voltage_range * (real_t) (f_record->fData[ f_samples ]) * f_voltage_inverse_levels;
                f_out[ t_index - f_next ] = t_datum;

                f_samples++;
            }
        }

        f_index = f_next + f_size;

        out< 0 >()->set_time( f_next * f_interval );

        return true;
    }

    bool rt_monarch_producer::stop_producer()
    {
        f_monarch->Close();
        delete f_monarch;
        f_monarch = NULL;
        f_header = NULL;
        f_record = NULL;

        f_length = 4194304;
        f_interval = 1.e-9;
        f_voltage_minimum = -.25;
        f_voltage_range = .5;
        f_voltage_inverse_range = 1. / .5;
        f_voltage_levels = 256.;
        f_voltage_inverse_levels = 1. / 256.;

        f_out = NULL;
        f_index = 0;
        f_next = 0;
        f_samples = 0;

        return true;
    }

    void rt_monarch_producer::finalize_producer()
    {
        return;
    }

}
