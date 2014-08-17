#include "types.hh"
#include "error.hh"
#include "utilmsg.hh"

static const string s_separator( "," );

using namespace midge;

int main( int p_argc, char** p_argv )
{
    if( p_argc < 2 )
    {
        utilmsg( s_warning ) << "usage:" << eom;
        utilmsg( s_warning ) << "  test_run_string_processing <test_string>" << eom;
        return -1;
    }
    string p_string( p_argv[ 1 ] );

    size_t t_start_pos;
    size_t t_separator_pos;
    string t_argument;
    string t_node;

    t_start_pos = 0;
    t_argument = p_string;
    while( true )
    {
        t_separator_pos = t_argument.find( s_separator, t_start_pos );

        t_node = t_argument.substr( t_start_pos, t_separator_pos - t_start_pos );
        t_argument = t_argument.substr( t_separator_pos + s_separator.size(), string_t::npos );

        if( t_node.size() == 0 )
        {
            throw error() << "root run found node name with length zero in argument <" << p_string << ">";
            return 0;
        }

        utilmsg( s_normal ) << "got node <" << t_node << ">" << eom;

        if( t_separator_pos == string_t::npos )
        {
            break;
        }
    }

    return 0;
}
