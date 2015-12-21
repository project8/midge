#include "message.hh"

#include <iomanip>
#include <ostream>
using std::cout;
using std::endl;

#include <cstdio>
#include <cstdlib>
#include <execinfo.h>

namespace midge
{
    mutex message::f_outer = mutex();
    mutex message::f_inner = mutex();
    set< pthread_t > message::f_threads = set< pthread_t >();

    message::message( const string& aKey, const string& aDescription, const string& aPrefix, const string& aSuffix ) :
            f_key( aKey ),
            f_system_description( aDescription ),
            f_system_prefix( aPrefix ),
            f_system_suffix( aSuffix ),

            f_error_color_prefix( "\33[31;1m" ),
            f_error_color_suffix( "\33[0m" ),
            f_error_description( "error" ),

            f_warning_color_prefix( "\33[33;1m" ),
            f_warning_color_suffix( "\33[0m" ),
            f_warning_description( "warning" ),

            f_normal_color_prefix( "\33[32;1m" ),
            f_normal_color_suffix( "\33[0m" ),
            f_normal_description( "normal" ),

            f_debug_color_prefix( "\33[36;1m" ),
            f_debug_color_suffix( "\33[0m" ),
            f_debug_description( "debug" ),

            f_default_color_prefix( "\33[37;1m" ),
            f_default_color_suffix( "\33[0m" ),
            f_default_description( "unknown" ),

            f_severity( s_normal ),

            f_color_prefix( &message::f_normal_color_prefix ),
            f_description( &message::f_normal_description ),
            f_color_suffix( &message::f_normal_color_suffix ),

            f_message_line(),
            f_message_lines(),

            f_terminal_severity( messages::get_instance()->get_terminal_severity() ),
            f_terminal_stream( messages::get_instance()->get_terminal_stream() ),
            f_log_severity( messages::get_instance()->get_log_severity() ),
            f_log_stream( messages::get_instance()->get_log_stream() )
    {
        f_message_line.setf( messages::get_instance()->get_format(), std::ios::floatfield );
        f_message_line.precision( messages::get_instance()->GetPrecision() );
        messages::get_instance()->add( this );
    }
    message::~message()
    {
        messages::get_instance()->remove( this );
    }

    const string& message::get_key() const
    {
        return f_key;
    }
    void message::set_key( const string& aKey )
    {
        f_key = aKey;
        return;
    }

    void message::set_severity( const message_severity& aSeverity )
    {
        f_severity = aSeverity;

        switch( f_severity )
        {
            case s_error :
                f_color_prefix = &message::f_error_color_prefix;
                f_description = &message::f_error_description;
                f_color_suffix = &message::f_error_color_suffix;
                break;

            case s_warning :
                f_color_prefix = &message::f_warning_color_prefix;
                f_description = &message::f_warning_description;
                f_color_suffix = &message::f_warning_color_suffix;
                break;

            case s_normal :
                f_color_prefix = &message::f_normal_color_prefix;
                f_description = &message::f_normal_description;
                f_color_suffix = &message::f_normal_color_suffix;
                break;

            case s_debug :
                f_color_prefix = &message::f_debug_color_prefix;
                f_description = &message::f_debug_description;
                f_color_suffix = &message::f_debug_color_suffix;
                break;

            default :
                f_color_prefix = &message::f_debug_color_prefix;
                f_description = &message::f_debug_description;
                f_color_suffix = &message::f_debug_color_suffix;
                break;
        }

        return;
    }
    void message::flush()
    {
        if( (f_severity <= f_terminal_severity) && (f_terminal_stream != NULL) && (f_terminal_stream->good() == true) )
        {
            for( vector< string >::iterator t_it = f_message_lines.begin(); t_it != f_message_lines.end(); t_it++ )
            {
                (*f_terminal_stream) << this->*f_color_prefix << f_system_prefix << "[" << f_system_description << " " << this->*f_description << " message] " << (*t_it) << f_system_suffix << this->*f_color_suffix << "\n";
            }
            (*f_terminal_stream).flush();
        }

        if( (f_severity <= f_log_severity) && (f_log_stream != NULL) && (f_log_stream->good() == true) )
        {
            for( vector< string >::iterator t_it = f_message_lines.begin(); t_it != f_message_lines.end(); t_it++ )
            {
                (*f_log_stream) << f_system_prefix << "[" << f_system_description << " " << this->*f_description << " message] " << *t_it << f_system_suffix << "\n";
            }
            (*f_log_stream).flush();
        }

        while( !f_message_lines.empty() )
        {
            f_message_lines.pop_back();
        }

        if( f_severity == s_error )
        {
            shutdown();
        }

        return;
    }

    void message::shutdown()
    {
        const size_t t_max_frames = 512;
        void* t_frame_array[ t_max_frames ];
        const size_t t_frame_count = backtrace( t_frame_array, t_max_frames );
        char** t_frame_symbols = backtrace_symbols( t_frame_array, t_frame_count );

        if( (f_severity <= f_terminal_severity) && (f_terminal_stream != NULL) && (f_terminal_stream->good() == true) )
        {
            (*f_terminal_stream) << this->*f_color_prefix << f_system_prefix << "[" << f_system_description << " " << this->*f_description << " message] shutting down..." << f_system_suffix << this->*f_color_suffix << '\n';
            (*f_terminal_stream) << this->*f_color_prefix << f_system_prefix << "[" << f_system_description << " " << this->*f_description << " message] stack trace:" << f_system_suffix << this->*f_color_suffix << '\n';
            for( size_t Index = 0; Index < t_frame_count; Index++ )
            {
                (*f_terminal_stream) << this->*f_color_prefix << f_system_prefix << "[" << f_system_description << " " << this->*f_description << " message] " << t_frame_symbols[ Index ] << f_system_suffix << this->*f_color_suffix << '\n';
            }
            (*f_terminal_stream).flush();
        }

        if( (f_severity <= f_log_severity) && (f_log_stream != NULL) && (f_log_stream->good() == true) )
        {
            (*f_log_stream) << f_system_prefix << "[" << f_system_description << " " << this->*f_description << " message] shutting down..." << f_system_suffix << '\n';
            (*f_log_stream) << f_system_prefix << "[" << f_system_description << " " << this->*f_description << " message] stack trace:" << f_system_suffix << '\n';
            for( size_t t_index = 0; t_index < t_frame_count; t_index++ )
            {
                (*f_log_stream) << f_system_prefix << "[" << f_system_description << " " << this->*f_description << " message] " << t_frame_symbols[ t_index ] << f_system_suffix << '\n';
            }
            (*f_log_stream).flush();
        }

        free( t_frame_symbols );
        exit( -1 );

        return;
    }

    void message::set_format( const message_format& aFormat )
    {
        f_message_line.setf( aFormat, std::ios::floatfield );
        return;
    }
    void message::set_precision( const message_precision& aPrecision )
    {
        f_message_line.precision( aPrecision );
        return;
    }
    void message::set_terminal_severity( const message_severity& aVerbosity )
    {
        f_terminal_severity = aVerbosity;
        return;
    }
    void message::set_terminal_stream( ostream* aTerminalStream )
    {
        f_terminal_stream = aTerminalStream;
        return;
    }
    void message::set_log_severity( const message_severity& aVerbosity )
    {
        f_log_severity = aVerbosity;
        return;
    }
    void message::set_log_stream( ostream* aLogStream )
    {
        f_log_stream = aLogStream;
        return;
    }

    void message::acquire()
    {
        bool_t t_flag;
        f_outer.lock();
        if( f_threads.find( pthread_self() ) == f_threads.end() )
        {
            f_threads.insert( pthread_self() );
            t_flag = true;
        }
        else
        {
            t_flag = false;
        }
        f_outer.unlock();

        if( t_flag == true )
        {
            f_inner.lock();
        }

        return;
    }
    void message::release()
    {
        bool_t t_flag;
        f_outer.lock();
        if( f_threads.find( pthread_self() ) == f_threads.end() )
        {
            t_flag = false;
        }
        else
        {
            f_threads.erase( pthread_self() );
            t_flag = true;
        }
        f_outer.unlock();

        if( t_flag == true )
        {
            f_inner.unlock();
        }

        return;
    }

}

namespace midge
{

    messages::messages() :
            f_map(),
            f_format( cout.flags() ),
            f_precision( cout.precision() ),
#ifdef MIDGE_ENABLE_DEBUG_MESSAGES
            f_terminal_severity( s_debug ),
#else
            f_terminal_severity( s_normal ),
#endif
            f_terminal_stream( &cout ),
#ifdef MIDGE_ENABLE_DEBUG_MESSAGES
            f_log_severity( s_debug ),
#else
            f_log_severity( s_normal ),
#endif
            f_log_stream( NULL )
    {
    }
    messages::~messages()
    {
    }

    void messages::add( message* aMessage )
    {
        it_t tIter = f_map.find( aMessage->get_key() );
        if( tIter == f_map.end() )
        {
            f_map.insert( entry_t( aMessage->get_key(), aMessage ) );
        }
        return;
    }
    message* messages::get( const string& aKey )
    {
        it_t tIter = f_map.find( aKey );
        if( tIter != f_map.end() )
        {
            return tIter->second;
        }
        return NULL;
    }
    void messages::remove( message* aMessage )
    {
        it_t tIter = f_map.find( aMessage->get_key() );
        if( tIter != f_map.end() )
        {
            f_map.erase( tIter );
        }
        return;
    }

    void messages::set_format( const message_format& aFormat )
    {
        f_format = aFormat;
        it_t tIter;
        for( tIter = f_map.begin(); tIter != f_map.end(); tIter++ )
        {
            tIter->second->set_format( f_format );
        }
        return;
    }
    const message_format& messages::get_format()
    {
        return f_format;
    }

    void messages::set_precision( const message_precision& aPrecision )
    {
        f_precision = aPrecision;
        it_t tIter;
        for( tIter = f_map.begin(); tIter != f_map.end(); tIter++ )
        {
            tIter->second->set_precision( f_precision );
        }
        return;
    }
    const message_precision& messages::GetPrecision()
    {
        return f_precision;
    }

    void messages::set_terminal_severity( const message_severity& aVerbosity )
    {
        f_terminal_severity = aVerbosity;
        it_t tIter;
        for( tIter = f_map.begin(); tIter != f_map.end(); tIter++ )
        {
            tIter->second->set_terminal_severity( f_terminal_severity );
        }
        return;
    }
    const message_severity& messages::get_terminal_severity()
    {
        return f_terminal_severity;
    }

    void messages::set_terminal_stream( ostream* aTerminalStream )
    {
        f_terminal_stream = aTerminalStream;
        it_t tIter;
        for( tIter = f_map.begin(); tIter != f_map.end(); tIter++ )
        {
            tIter->second->set_terminal_stream( f_terminal_stream );
        }
        return;
    }
    ostream* messages::get_terminal_stream()
    {
        return f_terminal_stream;
    }

    void messages::set_log_severity( const message_severity& aVerbosity )
    {
        f_log_severity = aVerbosity;
        it_t tIter;
        for( tIter = f_map.begin(); tIter != f_map.end(); tIter++ )
        {
            tIter->second->set_log_severity( f_log_severity );
        }
        return;
    }
    const message_severity& messages::get_log_severity()
    {
        return f_log_severity;
    }

    void messages::set_log_stream( ostream* aLogStream )
    {
        f_log_stream = aLogStream;
        it_t tIter;
        for( tIter = f_map.begin(); tIter != f_map.end(); tIter++ )
        {
            tIter->second->set_log_stream( f_log_stream );
        }
        return;
    }
    ostream* messages::get_log_stream()
    {
        return f_log_stream;
    }

}

message_define( msg, midge, midge )
