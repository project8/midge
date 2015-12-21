#include "typelength.hh"
#include "typelist.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

int main()
{

    typedef typelist_3( char, short, int ) list_one;

    cout << "length of list_one should be <3> and is <" << typelength< list_one >::result << ">" << endl;

    typedef typelist_10( unsigned char, char, unsigned short, short, unsigned int, int, unsigned long, long, float, double ) list_two;

    cout << "length of list_two should be <10> and is <" << typelength< list_two >::result << ">" << endl;

    return 0;

}
