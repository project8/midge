#include "types.hh"
#include "typelist.hh"
#include "typechain.hh"

#include <iostream>
using std::cout;
using std::endl;

namespace midge
{
    namespace test
    {
        template< class x_type, class x_index >
        class socket
        {
            public:
                socket( const string_t& p_name )
                {
                    cout << "<" << p_name << ">: creating socket for <" << typeid(x_type).name() << "> at index <" << x_index::result << ">" << endl;
                }
                ~socket()
                {
                }

        };

        template< class x_list >
        class parent :
            public typechain< socket< _1, _index >, x_list >
        {
            public:
                parent() :
                    typechain< socket< _1, _index >, x_list >( "parent" )
                {
                }
                ~parent()
                {
                }
        };
    }
}

using namespace midge;
using namespace midge::test;

int main()
{

    typedef typelist_3( char, short, int ) list_one;
    parent< list_one > t_parent_one;

    typedef typelist_10( unsigned char, char, unsigned short, short, unsigned int, int, unsigned long, long, float, double ) list_two;
    parent< list_two > t_parent_two;

    return 0;

}
