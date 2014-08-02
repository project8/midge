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
            f_index( 0 ),
            f_begin( 0 ),
            f_end( 0 )
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
        f_index = f_length;
        f_begin = 0;
        f_end = 0;

        return true;
    }

    bool rt_monarch_producer::execute_producer()
    {
        count_t t_index;
        real_t t_datum;

        if( f_end != 0 )
        {
            f_begin += f_stride;
        }

        if( f_end > f_begin )
        {
            for( t_index = f_begin; t_index < f_end; t_index++ )
            {
                f_out[ t_index - f_begin ] = f_out[ t_index - f_end + f_size ];
            }
            for( t_index = f_end; t_index < f_begin + f_size; t_index++ )
            {
                while( f_index >= f_length )
                {
                    if( f_monarch->ReadRecord() == false )
                    {
                        return false;
                    }
                    f_index -= f_length;
                }
                t_datum = f_voltage_minimum + f_voltage_range * (real_t) (f_record->fData[ f_index ]) * f_voltage_inverse_levels;
                f_out[ t_index - f_begin ] = t_datum;
                f_index++;
            }
        }
        else
        {
            for( t_index = f_begin; t_index < f_begin + f_size; t_index++ )
            {
                while( f_index >= f_length )
                {
                    if( f_monarch->ReadRecord() == false )
                    {
                        return false;
                    }
                    f_index -= f_length;
                }
                t_datum = f_voltage_minimum + f_voltage_range * (real_t) (f_record->fData[ f_index ]) * f_voltage_inverse_levels;
                f_out[ t_index - f_begin ] = t_datum;
                f_index++;
            }
        }

        f_end = f_begin + f_size;

        out< 0 >()->set_start_time( f_begin * f_interval );

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
        f_begin = 0;
        f_end = 0;

        return true;
    }

    void rt_monarch_producer::finalize_producer()
    {
        return;
    }

}
