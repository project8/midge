#include "rt_monarch_consumer.hh"

#include <cmath>

namespace midge
{

    rt_monarch_consumer::rt_monarch_consumer() :
            f_file( "" ),
            f_bits( 8 ),
            f_length( 4194304 ),
            f_duration( 1.e-3 ),
            f_voltage_minimum( -.25 ),
            f_voltage_maximum( .25 ),
            f_voltage_range( 1. ),
            f_voltage_inverse_range( 1. ),
            f_voltage_levels( 1. ),
            f_voltage_inverse_levels( 1. ),
            f_size( 0 ),
            f_interval( 1. ),
            f_in( NULL ),
            f_index( 0 ),
            f_samples( 0 ),
            f_records( 0 ),
            f_monarch( NULL ),
            f_header( NULL ),
            f_record( NULL )
    {
    }
    rt_monarch_consumer::~rt_monarch_consumer()
    {
    }

    void rt_monarch_consumer::set_file( const string& p_file )
    {
        f_file = p_file;
        return;
    }
    const string& rt_monarch_consumer::get_file() const
    {
        return f_file;
    }

    void rt_monarch_consumer::set_bits( const count_t& p_bits )
    {
        f_bits = p_bits;
        return;
    }
    const count_t& rt_monarch_consumer::get_bits() const
    {
        return f_bits;
    }

    void rt_monarch_consumer::set_length( const count_t& p_length )
    {
        f_length = p_length;
        return;
    }
    const count_t& rt_monarch_consumer::get_length() const
    {
        return f_length;
    }

    void rt_monarch_consumer::set_duration( const real_t& p_duration )
    {
        f_duration = p_duration;
        return;
    }
    const real_t& rt_monarch_consumer::get_duration() const
    {
        return f_duration;
    }

    void rt_monarch_consumer::set_voltage_minimum( const real_t& p_voltage_minimum )
    {
        f_voltage_minimum = p_voltage_minimum;
        return;
    }
    const real_t& rt_monarch_consumer::get_voltage_minimum() const
    {
        return f_voltage_minimum;
    }

    void rt_monarch_consumer::set_voltage_maximum( const real_t& p_voltage_maximum )
    {
        f_voltage_maximum = p_voltage_maximum;
        return;
    }
    const real_t& rt_monarch_consumer::get_voltage_maximum() const
    {
        return f_voltage_maximum;
    }

    void rt_monarch_consumer::initialize_consumer()
    {
        f_voltage_range = f_voltage_maximum - f_voltage_minimum;
        f_voltage_inverse_range = 1. / f_voltage_range;
        f_voltage_levels = 1 << f_bits;
        f_voltage_inverse_levels = 1. / f_voltage_levels;

        return;
    }

    bool rt_monarch_consumer::start_consumer()
    {
        f_size = in< 0 >()->get_size();
        f_interval = in< 0 >()->get_interval();
        f_in = in< 0 >()->raw();
        f_index = 0;
        f_samples = 0;
        f_records = 0;

        f_monarch = monarch::Monarch::OpenForWriting( f_file );

        f_header = f_monarch->GetHeader();
        f_header->SetFilename( f_file );
        f_header->SetDescription( string( "produced by midge rt monarch consumer <" ) + this->get_name() + string( ">" ) );
        f_header->SetFormatMode( monarch::sFormatSingle );
        f_header->SetAcquisitionMode( monarch::sOneChannel );
        f_header->SetAcquisitionRate( 1.e-6 / f_interval );
        f_header->SetRunDuration( 1.e3 * f_duration );
        f_header->SetRunType( monarch::sRunTypeSignal );
        f_header->SetRunSource( monarch::sSourceSimulation );
        f_header->SetRecordSize( f_length );
        f_header->SetDataTypeSize( sizeof(monarch::DataType) );
        f_header->SetBitDepth( f_bits );
        f_header->SetVoltageMin( f_voltage_minimum );
        f_header->SetVoltageRange( f_voltage_range );
        f_monarch->WriteHeader();

        f_monarch->SetInterface( monarch::sInterfaceInterleaved );
        f_record = f_monarch->GetRecordInterleaved();

        return true;
    }

    bool rt_monarch_consumer::execute_consumer()
    {
        count_t t_index;
        real_t t_datum;

        count_t t_next = (count_t) (round( in< 0 >()->get_time() / f_interval ));

        if( t_next < f_index )
        {
            for( t_index = f_index; t_index < t_next + f_size; t_index++ )
            {
                t_datum = f_in[ t_index - t_next ];
                t_datum = (t_datum - f_voltage_minimum) * f_voltage_inverse_range * f_voltage_levels;

                if( t_datum > (f_voltage_levels - 1.) )
                {
                    t_datum = f_voltage_levels - 1.;
                }
                else if( t_datum < 0. )
                {
                    t_datum = 0.;
                }

                f_record->fData[ f_samples ] = (monarch::DataType) (t_datum);

                if( ++f_samples == f_length )
                {
                    f_record->fAcquisitionId = 0;
                    f_record->fRecordId = f_records;
                    f_monarch->WriteRecord();
                    f_samples = 0;
                    f_records++;
                }
            }
        }
        else
        {
            for( t_index = t_next; t_index < t_next + f_size; t_index++ )
            {
                t_datum = f_in[ t_index - t_next ];
                t_datum = (t_datum - f_voltage_minimum) * f_voltage_inverse_range * f_voltage_levels;

                if( t_datum > (f_voltage_levels - 1.) )
                {
                    t_datum = f_voltage_levels - 1.;
                }
                else if( t_datum < 0. )
                {
                    t_datum = 0.;
                }

                f_record->fData[ f_samples ] = (monarch::DataType) (t_datum);

                if( ++f_samples == f_length )
                {
                    f_record->fAcquisitionId = 0;
                    f_record->fRecordId = f_records;
                    f_monarch->WriteRecord();
                    f_samples = 0;
                    f_records++;
                }
            }
        }

        f_index = t_next + f_size;

        if( f_index * f_interval >= f_duration )
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    bool rt_monarch_consumer::stop_consumer()
    {
        f_size = 0;
        f_interval = 1.;
        f_in = NULL;
        f_index = 0;
        f_samples = 0;
        f_records = 0;

        f_monarch->Close();
        delete f_monarch;
        f_monarch = NULL;
        f_header = NULL;
        f_record = NULL;

        return true;
    }

    void rt_monarch_consumer::finalize_consumer()
    {
        f_voltage_range = 1.;
        f_voltage_inverse_range = 1.;
        f_voltage_levels = 1;
        f_voltage_inverse_levels = 1.;

        return;
    }

}
