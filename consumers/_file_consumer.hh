#ifndef _midge__file_consumer_hh_
#define _midge__file_consumer_hh_

#include "consumer.hh"
#include "macros.hh"

namespace midge
{

    template< class x_data, class x_file >
    class _file_consumer :
        public _consumer< _file_consumer< x_data, x_file >, typelist_1( x_data )>
    {
        public:
            _file_consumer();
            virtual ~_file_consumer();

        public:
            accessible( string, file )

        public:
            void initialize();
            void execute();
            void finalize();

        private:
            typedef _consumer< _file_consumer< x_data, x_file >, typelist_1( x_data ) > parent;
    };

    template< class x_data, class x_file >
    _file_consumer< x_data, x_file >::_file_consumer() :
            f_file( "" )
    {
    }
    template< class x_data, class x_file >
    _file_consumer< x_data, x_file >::~_file_consumer()
    {
    }

    template< class x_data, class x_file >
    void _file_consumer< x_data, x_file >::initialize()
    {
        return;
    }

    template< class x_data, class x_file >
    void _file_consumer< x_data, x_file >::execute()
    {
        command_t t_command;
        x_data t_data;
        x_file t_file;

        while( true )
        {
            parent::template in_stream< 0 >() >> t_data;
            t_command = parent::template in_stream< 0 >().command();

            if( t_command == stream::s_start )
            {
                t_file.write( f_file );
                t_file << t_data;

                continue;
            }
            if( t_command == stream::s_run )
            {
                t_file << t_data;

                continue;
            }
            if( t_command == stream::s_stop )
            {
                t_file.close();

                continue;
            }
            if( t_command == stream::s_exit )
            {
                return;
            }
        }

        return;
    }

    template< class x_data, class x_file >
    void _file_consumer< x_data, x_file >::finalize()
    {
        return;
    }


}



#endif
