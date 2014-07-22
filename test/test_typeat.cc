#include "typelist.hh"
#include "typeat.hh"
using namespace midge;

#include <iostream>
using std::cout;
using std::endl;

int main()
{

    typedef typelist_3( char, short, int ) list_one;

    cout << "type at position <1> of list_one should be <short> and is <" << typeid(typeat< list_one, 2 >::result).name() << ">" << endl;
    cout << "type at position <5> of list_one should be <typenull> and is <" << typeid(typeat< list_one, 5 >::result).name() << ">" << endl;

    typedef typelist_10( unsigned char, char, unsigned short, short, unsigned int, int, unsigned long, long, float, double ) list_two;

    cout << "type at position <6> of list_one should be <unsigned long> and is <" << typeid(typeat< list_two, 6 >::result).name() << ">" << endl;
    cout << "type at position <0> of list_one should be <unsigned char> and is <" << typeid(typeat< list_two, 0 >::result).name() << ">" << endl;
    cout << "type at position <9> of list_one should be <real_t> and is <" << typeid(typeat< list_two, 9 >::result).name() << ">" << endl;

    return 0;

}
