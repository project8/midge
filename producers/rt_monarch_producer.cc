#include "rt_monarch_producer.hh"

#include <cmath>

namespace midge
{

    rt_monarch_producer::rt_monarch_producer() :
            f_file( "" ),
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
            f_start_index( 0 ),
            f_stop_index( 0 ),
            f_current_index( 0 ),
            f_record_index( 0 )
    {
    }
    rt_monarch_producer::~rt_monarch_producer()
    {
    }

    void rt_monarch_producer::set_file( const string& p_file )
    {
        f_file = p_file;
        return;
    }
    const string& rt_monarch_producer::get_file() const
    {
        return f_file;
    }

    void rt_monarch_producer::set_stride( const count_t& p_stride )
    {
        f_stride = p_stride;
        return;
    }
    const count_t& rt_monarch_producer::get_stride() const
    {
        return f_stride;
    }

    void rt_monarch_producer::set_size( const count_t& p_size )
    {
        f_size = p_size;
        return;
    }
    const count_t& rt_monarch_producer::get_size() const
    {
        return f_size;
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
        f_start_index = 0;
        f_stop_index = f_size;
        f_current_index = 0;
        f_record_index = f_length;

        return true;
    }

    bool rt_monarch_producer::execute_producer()
    {
        // shift the block
        for( count_t t_index = f_start_index; t_index < f_current_index; t_index++ )
        {
            f_out[ t_index - f_start_index ] = f_out[ t_index - f_start_index + f_stop_index - f_current_index ];
        }

        // add new data to the block
        real_t t_datum;
        for( count_t t_index = f_current_index; t_index < f_stop_index; t_index++ )
        {
            while( f_record_index >= f_length )
            {
                if( f_monarch->ReadRecord() == false )
                {
                    return false;
                }
                f_record_index -= f_length;
            }
            t_datum = f_voltage_minimum + f_voltage_range * (real_t) (f_record->fData[ f_record_index ]) * f_voltage_inverse_levels;

            f_out[ t_index - f_start_index ] = t_datum;
            f_record_index++;
        }

        out< 0 >()->set_start_time( f_start_index * f_interval );
        f_start_index += f_stride;
        f_current_index = f_stop_index;
        f_stop_index += f_stride;
        f_record_index += (f_stride > f_size ? f_stride - f_size : 0);

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
        f_start_index = 0;
        f_stop_index = 0;
        f_current_index = 0;
        f_record_index = 0;

        return true;
    }

    void rt_monarch_producer::finalize_producer()
    {
        return;
    }

}
