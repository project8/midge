#include "message.hh"
#include "binary.hh"

#include <fstream>
using std::fstream;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

namespace midge
{
    namespace test
    {
        class block
        {
            public:
                block() :
                        f_name( "" ),
                        f_parameters(),
                        f_flag( false )
                {
                    f_values[ 0 ][ 0 ] = 0.;
                    f_values[ 0 ][ 1 ] = 0.;
                    f_values[ 0 ][ 2 ] = 0.;
                    f_values[ 0 ][ 3 ] = 0.;
                    f_values[ 1 ][ 0 ] = 0.;
                    f_values[ 1 ][ 1 ] = 0.;
                    f_values[ 1 ][ 2 ] = 0.;
                    f_values[ 1 ][ 3 ] = 0.;

                }
                ~block()
                {
                }

                void print()
                {
                    msg( s_normal );
                    msg << "block:" << ret;
                    msg << "  name: " << f_name << ret;
                    msg << "  value[ 0 ][ 0 ]: " << f_values[ 0 ][ 0 ] << ret;
                    msg << "  value[ 0 ][ 1 ]: " << f_values[ 0 ][ 1 ] << ret;
                    msg << "  value[ 0 ][ 2 ]: " << f_values[ 0 ][ 2 ] << ret;
                    msg << "  value[ 0 ][ 3 ]: " << f_values[ 0 ][ 3 ] << ret;
                    msg << "  value[ 1 ][ 0 ]: " << f_values[ 1 ][ 0 ] << ret;
                    msg << "  value[ 1 ][ 1 ]: " << f_values[ 1 ][ 1 ] << ret;
                    msg << "  value[ 1 ][ 2 ]: " << f_values[ 1 ][ 2 ] << ret;
                    msg << "  value[ 1 ][ 3 ]: " << f_values[ 1 ][ 3 ] << ret;
                    for( count_t t_outer = 0; t_outer < f_parameters.size(); t_outer++ )
                    {
                        for( count_t t_inner = 0; t_inner < f_parameters[ t_outer ].size(); t_inner++ )
                        {
                            msg << "  parameter[ " << t_outer << " ][ " << t_inner << " ]: " << f_parameters[ t_outer ][ t_inner ] << ret;
                        }
                    }
                    msg << "  flag: " << (f_flag == true ? "true" : "false") << eom;

                    return;
                }

                typedef real_t value_t[ 4 ];

                string f_name;
                value_t f_values[ 2 ];
                vector< vector< uint16_t > > f_parameters;
                bool f_flag;
        };
    }

    template< >
    class binary::pull< test::block >
    {
        public:
            pull( binary& p_stream, test::block& p_block )
            {
                p_stream >> p_block.f_name;
                p_stream >> p_block.f_values[ 0 ];
                p_stream >> p_block.f_values[ 1 ];
                p_stream >> p_block.f_parameters;
                p_stream >> p_block.f_flag;
            }
    };

    template< >
    class binary::push< test::block >
    {
        public:
            push( binary& p_stream, const test::block& p_block )
            {
                p_stream << p_block.f_name;
                p_stream << p_block.f_values[ 0 ];
                p_stream << p_block.f_values[ 1 ];
                p_stream << p_block.f_parameters;
                p_stream << p_block.f_flag;
            }
    };

}

using namespace midge;
using namespace midge::test;

int main()
{

    binary t_binary;

    block t_writable;
    t_writable.f_name = string( "hello" );
    t_writable.f_values[ 0 ][ 0 ] = 12.3;
    t_writable.f_values[ 0 ][ 1 ] = 45.6;
    t_writable.f_values[ 0 ][ 2 ] = 78.9;
    t_writable.f_values[ 0 ][ 3 ] = 10.1;
    t_writable.f_values[ 1 ][ 0 ] = 22.3;
    t_writable.f_values[ 1 ][ 1 ] = 55.6;
    t_writable.f_values[ 1 ][ 2 ] = 88.9;
    t_writable.f_values[ 1 ][ 3 ] = 20.1;
    t_writable.f_flag = true;
    t_writable.f_parameters.resize( 2 );
    t_writable.f_parameters[ 0 ].resize( 2 );
    t_writable.f_parameters[ 0 ][ 0 ] = 255;
    t_writable.f_parameters[ 0 ][ 1 ] = 128;
    t_writable.f_parameters[ 1 ].resize( 1 );
    t_writable.f_parameters[ 1 ][ 0 ] = 76;

    t_writable.print();

    t_binary.write( "test_binary.mgb" );
    t_binary << t_writable;
    t_binary.close();

    block t_readable;

    t_binary.read( "test_binary.mgb" );
    t_binary >> t_readable;
    t_binary.close();

    t_readable.print();

    return 0;
}
