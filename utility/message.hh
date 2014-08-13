#ifndef _midge_message_hh_
#define _midge_message_hh_

#include "types.hh"
#include "mutex.hh"

#include <pthread.h>

#include <set>
using std::set;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <utility>
using std::pair;

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

#include <ostream>
using std::ostream;

#include <iomanip>
using std::setprecision;
using std::fixed;

namespace midge
{

    class message_line
    {
    };

    class message_end
    {
    };

    typedef std::ios_base::fmtflags message_format;
    typedef std::streamsize message_precision;
    typedef int message_severity;
    static const message_severity s_error = 0;
    static const message_severity s_warning = 1;
    static const message_severity s_normal = 2;
    static const message_severity s_debug = 3;
    static const message_line ret = message_line();
    static const message_end eom = message_end();

    class message
    {
        public:
            message( const string& aKey, const string& aDescription, const string& aPrefix, const string& aSuffix );
            virtual ~message();

        private:
            message();
            message( const message& );

            //**************
            //identification
            //**************

        public:
            void set_key( const string& aKey );
            const string& get_key() const;

        protected:
            string f_key;

            //*********
            //interface
            //*********

        public:
            message& operator()( const message_severity& );

            template< class XPrintable >
            message& operator<<( const XPrintable& p_fragment );
            message& operator<<( const message_line& );
            message& operator<<( const message_end& );

        private:
            void set_severity( const message_severity& aSeverity );
            void flush();
            void shutdown();

        protected:
            string f_system_description;
            string f_system_prefix;
            string f_system_suffix;

            string f_error_color_prefix;
            string f_error_color_suffix;
            string f_error_description;

            string f_warning_color_prefix;
            string f_warning_color_suffix;
            string f_warning_description;

            string f_normal_color_prefix;
            string f_normal_color_suffix;
            string f_normal_description;

            string f_debug_color_prefix;
            string f_debug_color_suffix;
            string f_debug_description;

            string f_default_color_prefix;
            string f_default_color_suffix;
            string f_default_description;

        private:
            message_severity f_severity;

            string message::*f_color_prefix;
            string message::*f_description;
            string message::*f_color_suffix;

            stringstream f_message_line;
            vector< string > f_message_lines;

            //********
            //settings
            //********

        public:
            void set_format( const message_format& aFormat );
            void set_precision( const message_precision& aPrecision );
            void set_terminal_severity( const message_severity& aVerbosity );
            void set_terminal_stream( ostream* aTerminalStream );
            void set_log_severity( const message_severity& aVerbosity );
            void set_log_stream( ostream* aLogStream );

        private:
            message_severity f_terminal_severity;
            ostream* f_terminal_stream;
            message_severity f_log_severity;
            ostream* f_log_stream;

            void acquire();
            void release();
            mutex f_outer;
            mutex f_inner;
            set< pthread_t > f_threads;
    };

    inline message& message::operator()( const message_severity& aSeverity )
    {
        acquire();

        set_severity( aSeverity );

        return *this;
    }

    template< class XPrintable >
    message& message::operator<<( const XPrintable& aFragment )
    {
        acquire();

        f_message_line << aFragment;

        return *this;
    }
    inline message& message::operator<<( const message_line& )
    {
        acquire();

        f_message_lines.push_back( f_message_line.str() );
        f_message_line.clear();
        f_message_line.str( "" );

        return *this;
    }
    inline message& message::operator<<( const message_end& )
    {
        acquire();

        f_message_lines.push_back( f_message_line.str() );
        f_message_line.clear();
        f_message_line.str( "" );
        flush();

        release();

        return *this;
    }

}

#include "singleton.hh"

namespace midge
{

    class messages :
        public singleton< messages >
    {

        public:
            friend class singleton< messages > ;

        private:
            messages();
            ~messages();

        public:
            void add( message* aMessage );
            message* get( const string& aKey );
            void remove( message* aMessage );

            void set_format( const message_format& aFormat );
            const message_format& get_format();

            void set_precision( const message_precision& aPrecision );
            const message_precision& GetPrecision();

            void set_terminal_severity( const message_severity& aVerbosity );
            const message_severity& get_terminal_severity();

            void set_terminal_stream( ostream* aTerminalStream );
            ostream* get_terminal_stream();

            void set_log_severity( const message_severity& aVerbosity );
            const message_severity& get_log_severity();

            void set_log_stream( ostream* aLogStream );
            ostream* get_log_stream();

        private:
            typedef map< string, message* > map_t;
            typedef map_t::value_type entry_t;
            typedef map_t::iterator it_t;
            typedef map_t::const_iterator cit_t;

            map_t f_map;

            message_format f_format;
            message_precision f_precision;
            message_severity f_terminal_severity;
            ostream* f_terminal_stream;
            message_severity f_log_severity;
            ostream* f_log_stream;
    };

}

#include "initializer.hh"

#define message_declare( x_name )\
namespace midge\
{\
    class message_ ## x_name :\
        public message\
    {\
        public:\
            message_ ## x_name();\
            virtual ~message_ ## x_name();\
    };\
\
    extern message_ ## x_name& x_name;\
    static initializer< message_ ## x_name > x_name ## _initializer;\
}

#define message_define( x_name, x_key, x_label )\
namespace midge\
{\
    message_ ## x_name::message_ ## x_name() :\
        message( #x_key, #x_label, "", "" )\
    {\
    }\
    message_ ## x_name::~message_ ## x_name()\
    {\
    }\
\
    message_ ## x_name& x_name = *((message_ ## x_name*) (initializer< message_ ## x_name >::f_data));\
}

#define message_define_full( x_name, x_key, x_label, x_prefix, x_suffix )\
namespace midge\
{\
    message_ ## x_name::message_ ## x_name() :\
        message( #x_key, #x_label, #x_prefix, #x_suffix )\
    {\
    }\
    message_ ## x_name::~message_ ## x_name()\
    {\
    }\
\
    message_ ## x_name& x_name = *((message_ ## x_name*) (initializer< message_ ## x_name >::f_data));\
}

#ifdef MIDGE_ENABLE_DEBUG_MESSAGES
#define msg_debug( x_name, x_content ) x_name( s_debug ) << x_content
#else
#define msg_debug( x_name, x_content )
#endif
#define msg_normal( x_name, x_content ) x_name( s_normal ) << x_content
#define msg_warning( x_name, x_content ) x_name( s_warning ) << x_content
#define msg_error( x_name, x_content ) x_name( s_error ) << x_content

#endif
