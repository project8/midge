#include "typeat.hh"

#include <iostream>

using namespace midge;

using std::cout;
using std::endl;

int main()
{
    cout << "in an variadic template, type at position 1 of <char, short, int> should be <short>, and is: " << typeid(type_at< 1, char, short, int >).name() << endl;

    //cout << "in an type_list, type at position 1 of <char, short, int> should be <short>, and is: " << typeid(type_at< 1, type_list<char, short, int> >).name() << endl;

    return 0;

}
