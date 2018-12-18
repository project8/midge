#include <iostream>

#include "typechain.hh"

#include "typename.hh"

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
                socket( const std::string& p_name )
                {
                    cout << "<" << p_name << ">: creating socket for <" << scarab::demangle(typeid(x_type).name()) << "> at index <" << x_index::value << ">" << endl;
                }
                ~socket()
                {
                }

        };

        template< class... x_types >
        class parent :
            public type_chain< socket< _type, _index >, 0, x_types... >
        {
            public:
                parent() :
                    type_chain< socket< _type, _index >, 0, x_types... >( "parent_2" )
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

    parent< char, short, int > t_parent_one;

    parent< unsigned char, char, unsigned short, short, unsigned int, int, unsigned long, long, float, double > t_parent_two;

    return 0;

}
