/*
 * test_slot.cc
 *
 *  Created on: Feb 6, 2019
 *      Author: N.S. Oblath
 */

#include "m_signal.hh"
#include "slot.hh"

#include <iostream>

namespace midge
{
    struct tester
    {
        void func( std::string arg1, int arg2 )
        {
            std::cout << "(member) " << arg1 << " " << arg2 << std::endl;
        }

        void const_func( std::string arg1, int arg2 )
        {
            std::cout << "(const member) " << arg1 << " " << arg2 << std::endl;
        }
    };
}

using namespace midge;

int main()
{
    // create new signal
    m_slot< std::string, int > lambda( "", [](std::string arg1, int arg2) {
        std::cout << "(lambda) " << arg1 << " " << arg2 << std::endl;
    } );

    tester the_tester;
    m_slot< std::string, int > member( "", &the_tester, &tester::func );

    m_slot< std::string, int > const_member( "", &the_tester, &tester::const_func );

    // create new signal
    m_signal< std::string, int > signal( "" );

    // attach a slot
    signal.connect( lambda.function() );
    signal.connect( member.function() );
    signal.connect( const_member.function() );

    signal.emit("The answer:", 42);

    return 0;
}


