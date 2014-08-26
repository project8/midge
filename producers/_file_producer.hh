#ifndef _midge__file_producer_hh_
#define _midge__file_producer_hh_

#include "producer.hh"
#include "macros.hh"

//#include "coremsg.hh"

namespace midge
{

    template< class x_data, class x_file >
    class _file_producer :
        public _producer< _file_producer< x_data, x_file >, typelist_1( x_data )>
    {
        public:
        _file_producer();
        virtual ~_file_producer();

        public:
        accessible( string, file )
        accessible( count_t, length )

        public:
        void initialize();
        void execute();
        void finalize();

        private:
        typedef _producer< _file_producer< x_data, x_file >, typelist_1( x_data ) > parent;
    };

    template< class x_data, class x_file >
    _file_producer< x_data, x_file >::_file_producer() :
            f_file( "" ),
            f_length( 10 )
    {
    }
    template< class x_data, class x_file >
    _file_producer< x_data, x_file >::~_file_producer()
    {
    }

    template< class x_data, class x_file >
    void _file_producer< x_data, x_file >::initialize()
    {
        parent::template out_buffer< 0 >().initialize( f_length );
        return;
    }

    template< class x_data, class x_file >
    void _file_producer< x_data, x_file >::execute()
    {
        x_data t_data;
        x_file t_file;

        t_file.read( f_file );

        parent::template out_stream< 0 >() >> t_data;
        t_file >> t_data;
        parent::template out_stream< 0 >().command( stream::s_start );
        parent::template out_stream< 0 >() << t_data;

        while( true )
        {
            if( t_file.end() == true )
            {
                t_file.close();

                parent::template out_stream< 0 >() >> t_data;
                parent::template out_stream< 0 >().command( stream::s_stop );
                parent::template out_stream< 0 >() << t_data;

                parent::template out_stream< 0 >() >> t_data;
                parent::template out_stream< 0 >().command( stream::s_exit );
                parent::template out_stream< 0 >() << t_data;

                return;
            }

            parent::template out_stream< 0 >() >> t_data;
            t_file >> t_data;
            parent::template out_stream< 0 >().command( stream::s_run );
            parent::template out_stream< 0 >() << t_data;
        }

        return;
    }

    template< class x_data, class x_file >
    void _file_producer< x_data, x_file >::finalize()
    {
        parent::template out_buffer< 0 >().finalize();
        return;
    }

}

#endif
