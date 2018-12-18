#include "typelength.hh"
#include "typelist.hh"

#include <iostream>

using namespace midge;

using std::cout;
using std::endl;

int main()
{

    typedef type_list< char, short, int > list_one;

    cout << "length of list_one should be <3> and is <" << type_size< list_one >::value << ">" << endl;

    typedef type_list< unsigned char, char, unsigned short, short, unsigned int, int, unsigned long, long, float, double > list_two;

    cout << "length of list_two should be <10> and is <" << type_size< list_two >::value << ">" << endl;

    return 0;

}
