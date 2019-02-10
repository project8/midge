/*
 * test_signal.cc
 *
 *  Created on: Dec 19, 2018
 *      Adapted by: N.S. Oblath
 *      Original author: Simon Schneegans
 *      License: public domain
 *      URL: http://simmesimme.github.io/tutorials/2015/09/20/signal-slot
 */

#include "signal_slot.hh"

#include <iostream>

using namespace midge;

int main()
{
    // create new signal
    m_signal< std::string, int > signal( "" );

    // attach a slot
    signal.connect_function( [](std::string arg1, int arg2) {
        std::cout << arg1 << " " << arg2 << std::endl;
    });

    signal.emit("The answer:", 42);

    return 0;
}
