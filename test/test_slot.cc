/*
 * test_slot.cc
 *
 *  Created on: Feb 6, 2019
 *      Author: N.S. Oblath
 */

#include "signal_slot.hh"

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
    // create new slots
    m_slot< std::string, int > lambda( "", [](std::string arg1, int arg2) {
        std::cout << "(lambda) " << arg1 << " " << arg2 << std::endl;
    } );

    tester the_tester;
    m_slot< std::string, int > member( "", &the_tester, &tester::func );

    m_slot< std::string, int > const_member( "", &the_tester, &tester::const_func );

    // create new signal
    m_signal< std::string, int > the_signal( "" );

    // attach a slot
    int lambda_conn = the_signal.connect( &lambda );
    int member_conn = the_signal.connect( &member );
    int const_member_conn = the_signal.connect( &const_member );

    std::cout << "All slots attached:" << std::endl;
    the_signal.emit( "The answer:", 42 );

    std::cout << "Disconnecting the lambda slot using signal::disconnect()" << std::endl;
    the_signal.disconnect( lambda_conn );
    the_signal.emit( "The answer:", 43 );

    std::cout << "Disconnecting the member slot using slot::disconnect_all()" << std::endl;
    member.disconnect_all();
    the_signal.emit( "The answer:", 44 );

    std::cout << "Disconnecting remaining member slots using signal::disconnect_all()" << std::endl;
    the_signal.disconnect_all();
    the_signal.emit( "The answer:", 45 );

    return 0;
}


